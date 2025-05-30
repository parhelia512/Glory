﻿using GloryEngine.Entities;
using System;
using System.Runtime.CompilerServices;

namespace GloryEngine.UI
{
    /// <summary>
    /// UI target
    /// </summary>
    public enum UITarget
    {
        /// <summary>
        /// No target, only renders the UI texture
        /// </summary>
        None,
        /// <summary>
        /// Overlay onto camera render
        /// </summary>
        CameraOverlay,
        /// <summary>
        /// 3D Quad in world space
        /// </summary>
        WorldSpaceQuad,
    }

    /// <summary>
    /// Resolution mode
    /// </summary>
    public enum ResolutionMode
    {
        /// <summary>
        /// Scale factor of the camera resolution
        /// </summary>
        CameraScale,
        /// <summary>
        /// Fixed resolution
        /// </summary>
        Fixed,
    }

    /// <summary>
    /// UI Renderer component
    /// </summary>
    public class UIRenderer : NativeComponent
    {
        #region Props

        /// <summary>
        /// The current instance of the UI document for rendering
        /// </summary>
        public UIScene RenderDocument
        {
            get
            {
                UInt64 docID = UIRenderer_GetRenderDocumentID(Object.Scene.ID, Object.ID, _objectID);
                if (docID == 0) return null;
                if (_document == null)
                    _document = new UIScene(this);
                _document.ID = docID;
                return _document;
            }
            private set { }
        }

        /// <summary>
        /// Document to render
        /// </summary>
        public UIDocument Document
        {
            get
            {
                UInt64 documentID = UIRenderer_GetDocumentID(Object.Scene.ID, Object.ID, _objectID);
                if (documentID == 0) return null;
                return Object.Scene.SceneManager.Engine.AssetManager.Get<UIDocument>(documentID);
            }
            set => UIRenderer_SetDocumentID(Object.Scene.ID, Object.ID, _objectID, value != null ? value.ID : 0);
        }

        /// <summary>
        /// Target to render the result to
        /// </summary>
        public UITarget Target
        {
            get => UIRenderer_GetTarget(Object.Scene.ID, Object.ID, _objectID);
            set => UIRenderer_SetTarget(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// How to handle resolution
        /// </summary>
        public ResolutionMode ResolutionMode
        {
            get => UIRenderer_GetResolutionMode(Object.Scene.ID, Object.ID, _objectID);
            set => UIRenderer_SetResolutionMode(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// Resolution of the UI texture
        /// </summary>
        public Vector2 Resolution
        {
            get => UIRenderer_GetResolution(Object.Scene.ID, Object.ID, _objectID).xy;
            set => UIRenderer_SetResolution(Object.Scene.ID, Object.ID, _objectID, new Vector3(value.x, value.y, 0.0f));
        }

        /// <summary>
        /// Material to use for world space rendering
        /// </summary>
        public Material WorldMaterial
        {
            get
            {
                UInt64 materialID = UIRenderer_GetWorldMaterialID(Object.Scene.ID, Object.ID, _objectID);
                if (materialID == 0) return null;
                return Object.Scene.SceneManager.Engine.AssetManager.Get<Material>(materialID);
            }
            set => UIRenderer_SetWorldMaterialID(Object.Scene.ID, Object.ID, _objectID, value != null ? value.ID : 0);
        }

        /// <summary>
        /// Size of the quad for world space rendering
        /// </summary>
        public Vector2 WorldSize
        {
            get => UIRenderer_GetWorldSize(Object.Scene.ID, Object.ID, _objectID).xy;
            set => UIRenderer_SetWorldSize(Object.Scene.ID, Object.ID, _objectID, new Vector3(value.x, value.y, 0.0f));
        }

        /// <summary>
        /// Whether input processing is allowed
        /// </summary>
        public bool InputEnabled
        {
            get => UIRenderer_GetInputEnabled(Object.Scene.ID, Object.ID, _objectID);
            set => UIRenderer_SetInputEnabled(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// Current input cursor position
        /// </summary>
        public Vector2 Cursor
        {
            get => UIRenderer_GetCursor(Object.Scene.ID, Object.ID, _objectID).xy;
            set => UIRenderer_SetCursor(Object.Scene.ID, Object.ID, _objectID, new Vector3(value.x, value.y, 0.0f));
        }

        /// <summary>
        /// Current input cursor down state
        /// </summary>
        public bool CursorDown
        {
            get => UIRenderer_GetCursorDown(Object.Scene.ID, Object.ID, _objectID);
            set => UIRenderer_SetCursorDown(Object.Scene.ID, Object.ID, _objectID, value);
        }

        #endregion

        #region Fields

        private UIScene _document;

        #endregion

        #region Methods

        /// <summary>
        /// Convert a world space position to a 2D position on the world
        /// space rendered quad that can be used as the cursor position for input.
        /// </summary>
        /// <param name="worldPos">World space position to convert</param>
        /// <returns>The converted 2D position on the UI document</returns>
        public Vector2 ConvertWorldToLocalPos(Vector3 worldPos) =>
            UIRenderer_ConvertWorldToLocalPos(Object.Scene.ID, Object.ID, _objectID, worldPos).xy;

        #endregion

        #region API Methods

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern UInt64 UIRenderer_GetRenderDocumentID(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern UInt64 UIRenderer_GetDocumentID(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetDocumentID(UInt64 sceneID, UInt64 objectID, UInt64 componentID, UInt64 documentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern UITarget UIRenderer_GetTarget(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetTarget(UInt64 sceneID, UInt64 objectID, UInt64 componentID, UITarget target);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ResolutionMode UIRenderer_GetResolutionMode(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetResolutionMode(UInt64 sceneID, UInt64 objectID, UInt64 componentID, ResolutionMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 UIRenderer_GetResolution(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetResolution(UInt64 sceneID, UInt64 objectID, UInt64 componentID, Vector3 resolution);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern UInt64 UIRenderer_GetWorldMaterialID(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetWorldMaterialID(UInt64 sceneID, UInt64 objectID, UInt64 componentID, UInt64 materialID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 UIRenderer_GetWorldSize(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetWorldSize(UInt64 sceneID, UInt64 objectID, UInt64 componentID, Vector3 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool UIRenderer_GetInputEnabled(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetInputEnabled(UInt64 sceneID, UInt64 objectID, UInt64 componentID, bool enabled);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 UIRenderer_GetCursor(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetCursor(UInt64 sceneID, UInt64 objectID, UInt64 componentID, Vector3 cursor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool UIRenderer_GetCursorDown(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void UIRenderer_SetCursorDown(UInt64 sceneID, UInt64 objectID, UInt64 componentID, bool down);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 UIRenderer_ConvertWorldToLocalPos(UInt64 sceneID, UInt64 objectID, UInt64 componentID, Vector3 worldPos);

        #endregion
    }
}
