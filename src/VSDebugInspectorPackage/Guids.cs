// Guids.cs
// MUST match guids.h
using System;

namespace DebugInspector
{
    static class GuidList
    {
        public const string guidVSDebugInspectorPackageString = "0ec51a7b-1e9f-4669-bf2f-8c147d892bd4";
        public const string guidVSPackage1CmdSetString = "4dfde259-bd9b-4035-8233-31c49e31be3e";
        public const string guidToolWindowPersistanceString = "fbd48652-c3e2-4b5e-bf94-816f18cf486f";

        public static readonly Guid guidVSPackage1CmdSet = new Guid(guidVSPackage1CmdSetString);
    };
}