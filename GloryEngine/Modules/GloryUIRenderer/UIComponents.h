#pragma once
#include "UIDocumentData.h"

#include <RenderData.h>
#include <Reflection.h>
#include <AssetReference.h>
#include <TextureData.h>
#include <FontData.h>
#include <MaterialData.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

REFLECTABLE_ENUM_NS(Glory, UITarget, None, CameraOverlay, WorldSpaceQuad);
REFLECTABLE_ENUM_NS(Glory, ResolutionMode, CameraScale, Fixed);

#define CONSTRAINT(axis)\
struct axis##Constraint\
{\
    static constexpr ConstraintAxis Axis = ConstraintAxis::axis;\
    axis##Constraint() : m_Constraint(0), m_Value(0.0f), m_FinalValue(0.0f) {}\
    axis##Constraint(float value) : m_Constraint(0), m_Value(value), m_FinalValue(0.0f) {}\
\
    explicit operator float() const { return m_FinalValue; }\
    explicit operator float() { return m_FinalValue; }\
    void operator=(float value) { m_Value = value; }\
\
    REFLECTABLE(axis##Constraint,\
        (uint32_t)(m_Constraint),\
        (float)(m_Value)\
    );\
    float m_FinalValue;\
};

namespace Glory
{
    /** @brief Constraint axis */
    enum ConstraintAxis
    {
        X,
        Y,
        Width,
        Height
    };

    /** @brief X Constraint */
    CONSTRAINT(X);
    /** @brief Y Constraint */
    CONSTRAINT(Y);
    /** @brief Width Constraint */
    CONSTRAINT(Width);
    /** @brief Height Constraint */
    CONSTRAINT(Height);

    /** @brief Transform for UI components */
    struct UITransform
    {
        UITransform() : m_X(0.0f), m_Y(0.0f), m_Width(100.0f), m_Height(100.0f),
            m_Pivot(0.0f, 0.0f), m_Rotation(0.0f), m_Scale(1.0f, 1.0f),
            m_ParentSize(), m_Transform(glm::identity<glm::mat4>()),
            m_TransformNoScale(glm::identity<glm::mat4>()),
            m_TransformNoScaleNoPivot(glm::identity<glm::mat4>()),
            m_InteractionTransform(glm::identity<glm::mat4>()),
            m_IsDirty(false) {}

        REFLECTABLE(UITransform,
            (XConstraint)(m_X),
            (YConstraint)(m_Y),
            (WidthConstraint)(m_Width),
            (HeightConstraint)(m_Height),
            (glm::vec2)(m_Pivot),
            (float)(m_Rotation),
            (glm::vec2)(m_Scale)
        );

        glm::vec2 m_ParentSize;
        glm::mat4 m_Transform;
        glm::mat4 m_TransformNoScale;
        glm::mat4 m_TransformNoScaleNoPivot;
        glm::mat4 m_InteractionTransform;
        glm::mat4 m_InteractionTransformNoPivot;
        bool m_IsDirty;
    };

    /** @brief UI Image renderer */
    struct UIImage
    {
        UIImage() : m_Image(0), m_Color(1.0f, 1.0f, 1.0f, 1.0f) {}

        REFLECTABLE(UIImage,
            (AssetReference<TextureData>)(m_Image),
            (glm::vec4)(m_Color)
        );
    };

    /** @brief UI Text renderer */
    struct UIText
    {
        UIText() : m_Font(0), m_Text("Hello World!"), m_Scale(1.0f),
            m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Alignment(Alignment::Left),
            m_Dirty(true) {}

        REFLECTABLE(UIText,
            (AssetReference<FontData>)(m_Font),
            (std::string)(m_Text),
            (std::string)(m_LocalizeTerm),
            (WidthConstraint)(m_Scale),
            (glm::vec4)(m_Color),
            (Alignment)(m_Alignment)
        );
        bool m_Dirty;
    };

    /** #brief UI Box renderer */
    struct UIBox
    {
        UIBox() : m_Color(1.0f, 1.0f, 1.0f, 1.0f) {}

        REFLECTABLE(UIBox,
            (glm::vec4)(m_Color)
        );
    };

    /** @brief UI Panel that crops child elements or can be used as an empty element */
    struct UIPanel
    {
        UIPanel() : m_Crop(true) {}

        REFLECTABLE(UIPanel,
            (bool)(m_Crop)
        );
    };

    /** @brief UI Interactions */
    struct UIInteraction
    {
        UIInteraction() : m_Enabled(true), m_Hovered(false), m_Down(false) {}

        REFLECTABLE(UIInteraction,
            (bool)(m_Enabled)
        );

        bool m_Hovered;
        bool m_Down;
    };

    /** @brief UI Document renderer component */
    struct UIRenderer
    {
        UIRenderer(): m_Target(UITarget::CameraOverlay),
            m_ResolutionMode(ResolutionMode::CameraScale),
            m_Resolution(1.0f, 1.0f), m_RenderDocumentID(0),
            m_WorldMaterial(0), m_WorldSize(1.0f, 1.0f),
            m_InputEnabled(true), m_CursorPos(0.0f, 0.0f),
            m_CursorDown(false), m_IsDirty(false)
        {}

        REFLECTABLE(UIRenderer,
            (AssetReference<UIDocumentData>)(m_Document),
            (UITarget)(m_Target),
            (ResolutionMode)(m_ResolutionMode),
            (glm::vec2)(m_Resolution),
            (AssetReference<MaterialData>)(m_WorldMaterial),
            (glm::vec2)(m_WorldSize),
            (bool)(m_InputEnabled)
        );

        UUID m_RenderDocumentID;
        glm::vec2 m_CursorPos;
        bool m_CursorDown;
        bool m_IsDirty;
    };
}
