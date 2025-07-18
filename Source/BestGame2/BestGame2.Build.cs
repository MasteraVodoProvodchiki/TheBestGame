// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class BestGame2 : ModuleRules
{
    public BestGame2(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[] { "Core", "CoreUObject", "Engine", "InputCore" });

        // ---------- Third‑Party: LicenseBulwark -------------------------
        //  ModuleDirectory  →  .../Source/BestGame2
        string ProtRoot = Path.Combine(ModuleDirectory, "..", "ThirdParty", "LicenseBulwark");

        // 1.  Заголовки
        PublicIncludePaths.Add(Path.Combine(ProtRoot, "include"));

        // 2.  Win64‑библиотеки
        string LibPath = Path.Combine(ProtRoot, "lib", "Win64");
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "LicCommon.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "LicSecure.lib"));

        // 3.  DLL для delay‑load и автокопирования
        PublicDelayLoadDLLs.Add("LicCommon.dll");
        RuntimeDependencies.Add("$(BinaryOutputDir)/LicCommon.dll",
            Path.Combine(LibPath, "LicCommon.dll"));

        // 4.  EXE‑утилита LicActivator
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string SrcExe = Path.Combine(ProtRoot, "tools", "LicActivator.exe");

            // для запуска из редактора / Standalone
            RuntimeDependencies.Add("$(BinaryOutputDir)/LicActivator.exe", SrcExe);

            // для финального Packaging
            RuntimeDependencies.Add("$(TargetOutputDir)/LicActivator.exe", SrcExe);
        }

        // (при необходимости добавьте ещё PrivateDependencyModuleNames и т.д.)
    }
}
