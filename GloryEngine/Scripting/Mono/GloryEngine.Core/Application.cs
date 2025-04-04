﻿using System.Runtime.CompilerServices;

namespace GloryEngine
{
    /// <summary>
    /// Application API
    /// </summary>
    public static class Application
    {
        #region Props

        /// <summary>
        /// Current version of the application, set in the General settings of the project
        /// </summary>
        public static Version Version => Application_GetVersion();

        #endregion

        #region Methods

        /// <summary>
        /// Close the application
        /// </summary>
        public static void Quit() => Application_Quit();

        #endregion

        #region API Methods

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Application_Quit();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static Version Application_GetVersion();

        #endregion
    }
}
