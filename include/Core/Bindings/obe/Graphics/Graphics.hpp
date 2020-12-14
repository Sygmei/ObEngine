#pragma once

namespace sol
{
    class state_view;
};
namespace obe::Graphics::Bindings
{
    void LoadClassColor(sol::state_view state);
    void LoadClassFont(sol::state_view state);
    void LoadClassPositionTransformer(sol::state_view state);
    void LoadClassRenderTarget(sol::state_view state);
    void LoadClassRichText(sol::state_view state);
    void LoadClassShader(sol::state_view state);
    void LoadClassSprite(sol::state_view state);
    void LoadClassSpriteHandlePoint(sol::state_view state);
    void LoadClassText(sol::state_view state);
    void LoadClassTexture(sol::state_view state);
    void LoadClassHsv(sol::state_view state);
    void LoadEnumColorType(sol::state_view state);
    void LoadEnumSpriteHandlePointType(sol::state_view state);
    void LoadFunctionInitPositionTransformer(sol::state_view state);
    void LoadFunctionMakeNullTexture(sol::state_view state);
    void LoadGlobalTransformers(sol::state_view state);
    void LoadGlobalParallax(sol::state_view state);
    void LoadGlobalCamera(sol::state_view state);
    void LoadGlobalPosition(sol::state_view state);
};