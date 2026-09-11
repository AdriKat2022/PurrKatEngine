#pragma once
#include "imgui.h"

// Code mostly stolen from imgui.h (ExampleAssetBrowser).

namespace PurrKatEngine
{
    struct SelectionData : ImGuiSelectionBasicStorage
    {
        // Find which item should be Focused after deletion.
        // Call _before_ item submission. Return an index in the before-deletion item list, your item loop should call SetKeyboardFocusHere() on it.
        // The subsequent ApplyDeletionPostLoop() code will use it to apply Selection.
        // - We cannot provide this logic in core Dear ImGui because we don't have access to selection data.
        // - We don't actually manipulate the ImVector<> here, only in ApplyDeletionPostLoop(), but using similar API for consistency and flexibility.
        // - Important: Deletion only works if the underlying ImGuiID for your items are stable: aka not depend on their index, but on e.g. item id/ptr.
        // FIXME-MULTISELECT: Doesn't take account of the possibility focus target will be moved during deletion. Need refocus or scroll offset.
        int ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count);

        // Rewrite item list (delete items) + update selection.
        // - Call after EndMultiSelect()
        // - We cannot provide this logic in core Dear ImGui because we don't have access to your items, nor to selection data.
        template <typename ITEM_TYPE>

        void ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, std::vector<ITEM_TYPE>& items, int item_curr_idx_to_select);
    };


    struct ElementItem
    {
        ImGuiID Id;
        int Type;
        std::string Name;
        std::string DisplayName;
        ImTextureRef ThumbnailTexture;

        static void SortWithSortSpecs(ImGuiTableSortSpecs* sort_specs, ElementItem* items, int items_count);

        static bool CompareWithSortSpecs(const ElementItem& a, const ElementItem& b, const ImGuiTableSortSpecs& sortSpecs);
    };
    
    struct ItemBrowser
    {
        using item_type = ElementItem;
        
        // Options
        bool Debugging = false;
        
        bool ShowTypeOverlay = true;
        bool AllowSorting = true;
        bool AllowBoxSelect = true; // Will set ImGuiMultiSelectFlags_BoxSelect2d
        bool AllowBoxSelectInsideSelection = false; // Will set ImGuiMultiSelectFlags_SelectOnClickAlways
        bool AllowDragUnselected = false; // Will set ImGuiMultiSelectFlags_SelectOnClickRelease
        float IconSize = 100;
        int IconSpacing = 10;
        int IconHitSpacing = 4; // Increase hit-spacing if you want to make it possible to clear or box-select from gaps. Some spacing is required to able to amend with Shift+box-select. Value is small in Explorer.
        bool StretchSpacing = true;
        bool UseScrollX = false; // Debug: submit twice the number of items per line (overflow horizontally to exercise ScrollX + box-select)

        // State
        std::vector<item_type> Items = {}; // Our items
        SelectionData Selection; // Our selection (ImGuiSelectionBasicStorage + helper funcs to handle deletion)
        ImGuiID NextItemId = 0; // Unique identifier when creating new items
        bool RequestDelete = false; // Deferred deletion request
        bool RequestSort = false; // Deferred sort request
        float ZoomWheelAccum = 0.0f; // Mouse wheel accumulator to handle smooth wheels better

        // Calculated sizes for layout, output of UpdateLayoutSizes(). Could be locals but our code is simpler this way.
        ImVec2 LayoutItemSize = ImVec2(40, 40);
        ImVec2 LayoutItemStep; // == LayoutItemSize + LayoutItemSpacing
        float LayoutItemSpacing = 0.0f;
        float LayoutSelectableSpacing = 0.0f;
        float LayoutOuterPadding = 0.0f;
        int LayoutColumnCount = 0;
        int LayoutLineCount = 0;

        // Thumbnails
        ImTextureRef DefaultThumbnail; // Default renderer ID for items without a thumbnail renderer. 

        // Functions
        std::function<void(item_type* item)> OnClick; // Callback function
        std::function<void(item_type* item)> OnDoubleClick; // Callback function

        void ReserveItems(int count);
        void AddItem(ElementItem item);
        void AddDebugItems(int count);

        void ClearItems();

        // Logic would be written in the main code BeginChild() and outputting to local variables.
        // We extracted it into a function so we can call it easily from multiple places.
        void UpdateLayoutSizes(float avail_width);

        void Draw(const char* title, bool* p_open);
    };
}
