#include "pkepch.h"
#include "ItemBrowser.h"

#include "imgui_internal.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    static std::string TruncateText(const std::string& text, float width)
    {
        std::string truncated_text = text;

        const float text_width = ImGui::CalcTextSize(text.c_str(), nullptr, true).x;

        if (text_width > width)
        {
            constexpr const char* ELLIPSIS = " ...";
            const float ellipsis_size = ImGui::CalcTextSize(ELLIPSIS).x;

            int visible_chars = 0;
            for (size_t i = 0; i < text.size(); i++)
            {
                const float current_width = ImGui::CalcTextSize(text.substr(0, i).c_str(), nullptr, true).x;
                if (current_width + ellipsis_size > width)
                    break;

                visible_chars = i;
            }

            truncated_text = (text.substr(0, visible_chars) + ELLIPSIS).c_str();
        }

        return truncated_text;
    }
    
    int SelectionData::ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count)
    {
        if (Size == 0)
            return -1;

        // If focused item is not selected...
        const int focused_idx = (int)ms_io->NavIdItem; // Index of currently focused item
        if (ms_io->NavIdSelected == false) // This is merely a shortcut, == Contains(adapter->IndexToStorage(items, focused_idx))
        {
            ms_io->RangeSrcReset = true; // Request to recover RangeSrc from NavId next frame. Would be ok to reset even when NavIdSelected==true, but it would take an extra frame to recover RangeSrc when deleting a selected item.
            return focused_idx; // Request to focus same item after deletion.
        }

        // If focused item is selected: land on first unselected item after focused item.
        for (int idx = focused_idx + 1; idx < items_count; idx++)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        // If focused item is selected: otherwise return last unselected item before focused item.
        for (int idx = MATHF_MIN(focused_idx, items_count) - 1; idx >= 0; idx--)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        return -1;
    }

    template<typename ITEM_TYPE>
    void SelectionData::ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, std::vector<ITEM_TYPE>& items, int item_curr_idx_to_select)
    {
        // Rewrite item list (delete items) + convert old selection index (before deletion) to new selection index (after selection).
        // If NavId was not part of selection, we will stay on same item.
        std::vector<ITEM_TYPE> new_items;
        new_items.reserve(items.size() - Size);
        int item_next_idx_to_select = -1;
        for (int idx = 0; idx < items.size(); idx++)
        {
            if (!Contains(GetStorageIdFromIndex(idx)))
                new_items.push_back(items[idx]);
            if (item_curr_idx_to_select == idx)
                item_next_idx_to_select = new_items.size() - 1;
        }
        items.swap(new_items);

        // Update selection
        Clear();
        if (item_next_idx_to_select != -1 && ms_io->NavIdSelected)
            SetItemSelected(GetStorageIdFromIndex(item_next_idx_to_select), true);
    }

    void ElementItem::SortWithSortSpecs(ImGuiTableSortSpecs* sort_specs, ElementItem* items, int items_count)
    {
        if (items_count > 1)
            std::sort(items, items + items_count, [sort_specs](const ElementItem& a, const ElementItem& b)
            {
                return CompareWithSortSpecs(a, b, *sort_specs);
            });
    }

    bool ElementItem::CompareWithSortSpecs(const ElementItem& a, const ElementItem& b, const ImGuiTableSortSpecs& sortSpecs)
    {
        for (int n = 0; n < sortSpecs.SpecsCount; ++n)
        {
            const ImGuiTableColumnSortSpecs& sort_spec = sortSpecs.Specs[n];

            int delta = 0;

            if (sort_spec.ColumnIndex == 0)
            {
                delta = (a.Name > b.Name) - (a.Name < b.Name);
            }
            else if (sort_spec.ColumnIndex == 1)
            {
                delta = (a.Type > b.Type) - (a.Type < b.Type);
            }

            if (delta != 0)
            {
                if (sort_spec.SortDirection == ImGuiSortDirection_Ascending)
                    return delta < 0;

                return delta > 0;
            }
        }

        return a.Id < b.Id;
    }
    
    void ItemBrowser::ReserveItems(int count)
    {
        Items.reserve(Items.size() + count);
    }

    void ItemBrowser::AddItem(ElementItem item)
    {
        item.Id = NextItemId++;

        if (item.DisplayName.empty())
            item.DisplayName = item.Name;
        
        Items.push_back(item);
        RequestSort = true;
    }
    
    void ItemBrowser::AddDebugItems(int count)
    {
        Items.reserve(Items.size() + count);
        for (int n = 0; n < count; n++, NextItemId++)
            Items.push_back(ElementItem{
                .Id = NextItemId,
                .Type = (NextItemId % 20) < 15 ? 0 : (NextItemId % 20) < 18 ? 1 : 2,
                .ThumbnailTexture = DefaultThumbnail
            });
        RequestSort = true;
    }

    void ItemBrowser::ClearItems()
    {
        // NextItemId = 0;
        Items.clear();
        Selection.Clear();
    }

    void ItemBrowser::UpdateLayoutSizes(float avail_width)
    {
        // Layout: when not stretching: allow extending into right-most spacing.
        LayoutItemSpacing = (float)IconSpacing;
        if (StretchSpacing == false)
            avail_width += floorf(LayoutItemSpacing * 0.5f);

        // Layout: calculate number of icon per line and number of lines
        LayoutItemSize = ImVec2(floorf(IconSize), floorf(IconSize));
        LayoutColumnCount = MATHF_MAX((int)(avail_width / (LayoutItemSize.x + LayoutItemSpacing)), 1);

        // Layout: when stretching: allocate remaining space to more spacing. Round before division, so item_spacing may be non-integer.
        if (StretchSpacing && LayoutColumnCount > 1)
            LayoutItemSpacing = floorf(avail_width - LayoutItemSize.x * LayoutColumnCount) / LayoutColumnCount;

        if (UseScrollX)
            LayoutColumnCount *= 2;
        LayoutLineCount = (Items.size() + LayoutColumnCount - 1) / LayoutColumnCount;

        LayoutItemStep = ImVec2(LayoutItemSize.x + LayoutItemSpacing, LayoutItemSize.y + LayoutItemSpacing);
        LayoutSelectableSpacing = MATHF_MAX(floorf(LayoutItemSpacing) - IconHitSpacing, 0.0f);
        LayoutOuterPadding = floorf(LayoutItemSpacing * 0.5f);
    }

    void ItemBrowser::Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(IconSize * 25, IconSize * 15), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (Debugging)
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Add 10000 items"))
                        AddDebugItems(10000);
                    if (ImGui::MenuItem("Clear items"))
                        ClearItems();
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                        *p_open = false;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                        RequestDelete = true;
                    ImGui::EndMenu();
                }
            }
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::PushItemWidth(ImGui::GetFontSize() * 10);

                ImGui::SeparatorText("Contents");
                ImGui::Checkbox("Show Type Overlay", &ShowTypeOverlay);
                ImGui::Checkbox("Allow Sorting", &AllowSorting);

                ImGui::SeparatorText("Selection Behavior");
                ImGui::Checkbox("Allow box-selection", &AllowBoxSelect);
                if (ImGui::Checkbox("Allow box-selection from selected items", &AllowBoxSelectInsideSelection) && AllowBoxSelectInsideSelection)
                    AllowDragUnselected = false;
                if (ImGui::Checkbox("Allow dragging unselected item", &AllowDragUnselected) && AllowDragUnselected)
                    AllowBoxSelectInsideSelection = false;

                ImGui::SeparatorText("Layout");
                ImGui::SliderFloat("Icon Size", &IconSize, 16.0f, 128.0f, "%.0f");
                ImGui::SameLine();
                ImGui::SliderInt("Icon Spacing", &IconSpacing, 0, 32);
                ImGui::SliderInt("Icon Hit Spacing", &IconHitSpacing, 0, 32);
                ImGui::Checkbox("Stretch Spacing", &StretchSpacing);
                ImGui::Checkbox("Use ScrollX", &UseScrollX);
                ImGui::PopItemWidth();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Show a table with ONLY one header row to showcase the idea/possibility of using this to provide a sorting UI
        if (AllowSorting)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiTableFlags table_flags_for_sort_specs = ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders;
            if (ImGui::BeginTable("for_sort_specs_only", 2, table_flags_for_sort_specs, ImVec2(0.0f, ImGui::GetFrameHeight())))
            {
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();
                if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                    if (sort_specs->SpecsDirty || RequestSort)
                    {
                        ElementItem::SortWithSortSpecs(sort_specs, Items.data(), Items.size());
                        sort_specs->SpecsDirty = RequestSort = false;
                    }
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowContentSize(ImVec2(0.0f, LayoutOuterPadding + LayoutLineCount * (LayoutItemSize.y + LayoutItemSpacing)));
        if (ImGui::BeginChild("Assets", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            const float avail_width = ImGui::GetContentRegionAvail().x;
            UpdateLayoutSizes(avail_width);

            // Calculate and store start position.
            ImVec2 start_pos = ImGui::GetCursorScreenPos();
            start_pos = ImVec2(start_pos.x + LayoutOuterPadding, start_pos.y + LayoutOuterPadding);
            ImGui::SetCursorScreenPos(start_pos);

            // Multi-select
            ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid;

            // - Enable box-select (in 2D mode, so that changing box-select rectangle X1/X2 boundaries will affect clipped items)
            if (AllowBoxSelect)
                ms_flags |= ImGuiMultiSelectFlags_BoxSelect2d;

            // - Selection mode
            if (AllowDragUnselected)
                ms_flags |= ImGuiMultiSelectFlags_SelectOnClickRelease; // Rarely used: Allows dragging an unselected item without selecting it(rarely used)
            else if (AllowBoxSelectInsideSelection)
                ms_flags |= ImGuiMultiSelectFlags_SelectOnClickAlways; // Rarely used: Prevents Drag and Drop from being used on multiple-selection, but allows e.g. BoxSelect to always reselect even when clicking inside an existing selection.

            // - Enable keyboard wrapping on X axis
            // (FIXME-MULTISELECT: We haven't designed/exposed a general nav wrapping api yet, so this flag is provided as a courtesy to avoid doing:
            //    ImGui::NavMoveRequestTryWrapping(ImGui::GetCurrentWindow(), ImGuiNavMoveFlags_WrapX);
            // When we finish implementing a more general API for this, we will obsolete this flag in favor of the new system)
            ms_flags |= ImGuiMultiSelectFlags_NavWrapX;

            ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, Selection.Size, Items.size());

            // Use custom selection adapter: store ID in selection (recommended)
            Selection.UserData = this;
            Selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx)
            {
                ItemBrowser* self = (ItemBrowser*)self_->UserData;
                return self->Items[idx].Id;
            };
            Selection.ApplyRequests(ms_io);

            const bool want_delete = (ImGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (Selection.Size > 0)) || RequestDelete;
            const int item_curr_idx_to_focus = want_delete ? Selection.ApplyDeletionPreLoop(ms_io, Items.size()) : -1;
            RequestDelete = false;

            // Push LayoutSelectableSpacing (which is LayoutItemSpacing minus hit-spacing, if we decide to have hit gaps between items)
            // Altering style ItemSpacing may seem unnecessary as we position every items using SetCursorScreenPos()...
            // But it is necessary for two reasons:
            // - Selectables uses it by default to visually fill the space between two items.
            // - The vertical spacing would be measured by Clipper to calculate line height if we didn't provide it explicitly (here we do).
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, LayoutSelectableSpacing));

            // Rendering parameters
            const ImU32 icon_type_overlay_colors[3] = {0, IM_COL32(200, 70, 70, 255), IM_COL32(70, 170, 70, 255)};
            const ImU32 icon_bg_color = ImGui::GetColorU32(IM_COL32(35, 35, 35, 220));
            const ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
            const bool display_label = (LayoutItemSize.x >= ImGui::CalcTextSize("XXX").x);

            const int column_count = LayoutColumnCount;
            ImGuiListClipper clipper;
            clipper.Begin(LayoutLineCount, LayoutItemStep.y);
            if (item_curr_idx_to_focus != -1)
                clipper.IncludeItemByIndex(item_curr_idx_to_focus / column_count); // Ensure focused item line is not clipped.
            if (ms_io->RangeSrcItem != -1)
                clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem / column_count); // Ensure RangeSrc item line is not clipped.
            while (clipper.Step())
            {
                for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++)
                {
                    const int item_min_idx_for_current_line = line_idx * column_count;
                    const int item_max_idx_for_current_line = MATHF_MIN((line_idx + 1) * column_count, Items.size());
                    for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx)
                    {
                        if (item_idx >= Items.size())
                            break;
                        
                        ElementItem* item_data = &Items[item_idx];
                        
                        ImGui::PushID((int)item_data->Id);

                        // Position item
                        ImVec2 pos = ImVec2(start_pos.x + (item_idx % column_count) * LayoutItemStep.x, start_pos.y + line_idx * LayoutItemStep.y);
                        ImGui::SetCursorScreenPos(pos);

                        ImGui::SetNextItemSelectionUserData(item_idx);
                        bool item_is_selected = Selection.Contains(item_data->Id);
                        bool item_is_visible = ImGui::IsRectVisible(LayoutItemSize);
                        if (ImGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, LayoutItemSize))
                        {
                            if (OnClick != nullptr)
                                OnClick(item_data);
                        }
                        
                        // Double click
                        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                        {
                            if (OnDoubleClick != nullptr)
                                OnDoubleClick(item_data);
                        }

                        // Update our selection state immediately (without waiting for EndMultiSelect() requests)
                        // because we use this to alter the color of our text/icon.
                        if (ImGui::IsItemToggledSelection())
                            item_is_selected = !item_is_selected;

                        // Focus (for after deletion)
                        if (item_curr_idx_to_focus == item_idx)
                            ImGui::SetKeyboardFocusHere(-1);

                        // Drag and drop
                        if (ImGui::BeginDragDropSource())
                        {
                            // Create payload with full selection OR single unselected item.
                            // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
                            if (ImGui::GetDragDropPayload() == NULL)
                            {
                                ImVector<ImGuiID> payload_items;
                                void* it = NULL;
                                ImGuiID id = 0;
                                if (!item_is_selected)
                                    payload_items.push_back(item_data->Id);
                                else
                                    while (Selection.GetNextSelectedItem(&it, &id))
                                        payload_items.push_back(id);
                                ImGui::SetDragDropPayload("ASSETS_BROWSER_ITEMS", payload_items.Data, (size_t)payload_items.size_in_bytes());
                            }

                            // Display payload content in tooltip, by extracting it from the payload data
                            // (we could read from selection, but it is more correct and reusable to read from payload)
                            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
                            const int payload_count = (int)payload->DataSize / (int)sizeof(ImGuiID);
                            ImGui::Text("%d assets", payload_count);

                            ImGui::EndDragDropSource();
                        }

                        // Render icon
                        // Because we use ImGuiMultiSelectFlags_BoxSelect2d, clipping vertical may occasionally be larger, so we coarse-clip our rendering as well.
                        if (item_is_visible)
                        {
                            ImVec2 box_min(pos.x - 1, pos.y - 1);
                            ImVec2 box_max(box_min.x + LayoutItemSize.x + 2, box_min.y + LayoutItemSize.y + 2);
                            const float pad = 4.0f, label_h = ImGui::GetFontSize(), width = box_max.x - box_min.x;
                            const float icon_size = Math::Min(width - pad * 2.0f, box_max.y - box_min.y - label_h - pad * 2.0f);
                            ImVec2 icon_min(box_min.x + (width - icon_size) * 0.5f, box_min.y + pad);
                            ImVec2 icon_max(icon_min.x + icon_size, icon_min.y + icon_size);

                            if (item_data->ThumbnailTexture.GetTexID() == 0)
                                draw_list->AddRectFilled(icon_min, icon_max, icon_bg_color);
                            else
                                draw_list->AddImage(item_data->ThumbnailTexture, icon_min, icon_max, ImVec2(0, 1), ImVec2(1, 0));

                            if (ShowTypeOverlay && item_data->Type != 0)
                            {
                                ImU32 type_col = icon_type_overlay_colors[item_data->Type % IM_COUNTOF(icon_type_overlay_colors)];
                                draw_list->AddRectFilled(ImVec2(icon_max.x - 2.0f - icon_type_overlay_size.x, icon_min.y + 2.0f), ImVec2(icon_max.x - 2.0f, icon_min.y + 2.0f + icon_type_overlay_size.y), type_col);
                            }

                            if (display_label)
                            {
                                const ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
                                
                                std::string display = TruncateText(item_data->DisplayName, width);
                                const ImVec2 display_size = ImGui::CalcTextSize(display.c_str());
                                draw_list->AddText(ImVec2(box_min.x + (width - display_size.x) * 0.5f, icon_max.y + pad), label_col, display.c_str());
                            }
                        }
                        ImGui::PopID();
                    }
                }
            }
            clipper.End();
            if (Items.size() == 0)
                ImGui::Dummy(ImVec2(0, 0));
            ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

            // Context menu
            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::Text("Selection: %d items", Selection.Size);
                ImGui::Separator();
                if (ImGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                    RequestDelete = true;
                ImGui::EndPopup();
            }

            ms_io = ImGui::EndMultiSelect();
            Selection.ApplyRequests(ms_io);
            if (want_delete)
                Selection.ApplyDeletionPostLoop(ms_io, Items, item_curr_idx_to_focus);

            // Zooming with Ctrl+Wheel
            if (ImGui::IsWindowAppearing())
                ZoomWheelAccum = 0.0f;
            if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsAnyItemActive() == false)
            {
                ZoomWheelAccum += io.MouseWheel;
                if (fabsf(ZoomWheelAccum) >= 1.0f)
                {
                    // Calculate hovered item index from mouse location
                    // FIXME: Locking aiming on 'hovered_item_idx' (with a cool-down timer) would ensure zoom keeps on it.
                    const float hovered_item_nx = (io.MousePos.x - start_pos.x + LayoutItemSpacing * 0.5f) / LayoutItemStep.x;
                    const float hovered_item_ny = (io.MousePos.y - start_pos.y + LayoutItemSpacing * 0.5f) / LayoutItemStep.y;
                    const int hovered_item_idx = ((int)hovered_item_ny * LayoutColumnCount) + (int)hovered_item_nx;
                    //ImGui::SetTooltip("%f,%f -> item %d", hovered_item_nx, hovered_item_ny, hovered_item_idx); // Move those 4 lines in block above for easy debugging

                    // Zoom
                    IconSize *= powf(1.1f, (float)(int)ZoomWheelAccum);
                    IconSize = MATHF_CLAMP(IconSize, 16.0f, 128.0f);
                    ZoomWheelAccum -= (float)(int)ZoomWheelAccum;
                    UpdateLayoutSizes(avail_width);

                    // Manipulate scroll to that we will land at the same Y location of currently hovered item.
                    // - Calculate next frame position of item under mouse
                    // - Set new scroll position to be used in next ImGui::BeginChild() call.
                    float hovered_item_rel_pos_y = ((float)(hovered_item_idx / LayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * LayoutItemStep.y;
                    hovered_item_rel_pos_y += ImGui::GetStyle().WindowPadding.y;
                    float mouse_local_y = io.MousePos.y - ImGui::GetWindowPos().y;
                    ImGui::SetScrollY(hovered_item_rel_pos_y - mouse_local_y);
                }
            }
        }
        ImGui::EndChild();

        ImGui::Text("Selected: %d/%d items", Selection.Size, Items.size());
        ImGui::End();
    }
}
