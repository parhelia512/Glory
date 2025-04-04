﻿using System;
using System.Runtime.CompilerServices;

namespace GloryEngine.Entities
{
    /// <summary>
    /// Text alignment
    /// </summary>
    public enum Alignment
    {
        /// <summary>
        /// Left alignment
        /// </summary>
        Left,
        /// <summary>
        /// Center alignment
        /// </summary>
        Center,
        /// <summary>
        /// Right alignment
        /// </summary>
        Right
    }

    /// <summary>
    /// Handle for a TextComponent component
    /// </summary>
    public class TextComponent : NativeComponent
    {
        #region Props

        /// <summary>
        /// Font to use for rendering
        /// </summary>
        public Font Font
        {
            get
            {
                UInt64 fontID = TextComponent_GetFont(Object.Scene.ID, Object.ID, _objectID);
                if (fontID == 0) return null;
                return Object.Scene.SceneManager.Engine.AssetManager.Get<Font>(fontID);
            }
            set => TextComponent_SetFont(Object.Scene.ID, Object.ID, _objectID, value != null ? value.ID : 0);
        }

        /// <summary>
        /// Text to render
        /// </summary>
        public string Text
        {
            get => TextComponent_GetText(Object.Scene.ID, Object.ID, _objectID);
            set => TextComponent_SetText(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// Scale to render the text at
        /// </summary>
        public float Scale
        {
            get => TextComponent_GetScale(Object.Scene.ID, Object.ID, _objectID);
            set => TextComponent_SetScale(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// Color of the text
        /// </summary>
        public Vector4 Color
        {
            get => TextComponent_GetColor(Object.Scene.ID, Object.ID, _objectID);
            set => TextComponent_SetColor(Object.Scene.ID, Object.ID, _objectID, ref value);
        }

        /// <summary>
        /// Text alignment
        /// </summary>
        public Alignment Alignment
        {
            get => TextComponent_GetAlignment(Object.Scene.ID, Object.ID, _objectID);
            set => TextComponent_SetAlignment(Object.Scene.ID, Object.ID, _objectID, value);
        }

        /// <summary>
        /// Automatic wrapping width thresshold, set to 0 for no wrapping
        /// </summary>
        public float WrapWidth
        {
            get => TextComponent_GetWrapWidth(Object.Scene.ID, Object.ID, _objectID);
            set => TextComponent_SetWrapWidth(Object.Scene.ID, Object.ID, _objectID, value);
        }

        #endregion

        #region API Methods

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static UInt64 TextComponent_GetFont(UInt64 sceneID, UInt64 objectID, UInt64 componentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetFont(UInt64 sceneID, UInt64 objectID, UInt64 componentID, UInt64 fontID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static string TextComponent_GetText(UInt64 sceneID, UInt64 objectID, UInt64 componentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetText(UInt64 sceneID, UInt64 objectID, UInt64 componentID, string str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static float TextComponent_GetScale(UInt64 sceneID, UInt64 objectID, UInt64 componentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetScale(UInt64 sceneID, UInt64 objectID, UInt64 componentID, float size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static Vector4 TextComponent_GetColor(UInt64 sceneID, UInt64 objectID, UInt64 componentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetColor(UInt64 sceneID, UInt64 objectID, UInt64 componentID, ref Vector4 color);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetAlignment(UInt64 sceneID, UInt64 objectID, UInt64 componentID, Alignment alignment);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static Alignment TextComponent_GetAlignment(UInt64 sceneID, UInt64 objectID, UInt64 componentID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void TextComponent_SetWrapWidth(UInt64 sceneID, UInt64 objectID, UInt64 componentID, float wrap);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static float TextComponent_GetWrapWidth(UInt64 sceneID, UInt64 objectID, UInt64 componentID);

        #endregion
    }
}
