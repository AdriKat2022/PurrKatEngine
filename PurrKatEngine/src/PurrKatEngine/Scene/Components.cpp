#include "pkepch.h"
#include "Components.h"

#include "Entity.h"

namespace PurrKatEngine
{
    void CameraComponent::OnMount(Entity& entity)
    {
        // Reset the camera to default values.
        Camera = SceneCamera();
        Camera.SetViewportSize(entity.GetScene()->GetViewportWidth(), entity.GetScene()->GetViewportHeight());
    }
}
