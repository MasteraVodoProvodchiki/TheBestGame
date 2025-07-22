// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
//#include <Windows.h>

PROTECT_FUNC(secureHello, {
    //volatile int _dummy = __COUNTER__;
    //for(int i = 0; i < 1000; ++i) {
    //    _dummy += i; // Dummy operation to prevent optimization
	//}
    // This function is protected by the license system
    UE_LOG(LogTemp, Log, TEXT("222-UTSGameInstance::secureHello — Hello!"));
})

/*
#define ENC_SEG(cls, fn) __declspec(code_seg(".enc_" #cls "_" #fn)) __declspec(noinline)

__pragma(section(".enc_sec", execute, read))
__pragma(comment(linker, "/SECTION:.enc_sec,ERW"))
//__pragma(code_seg(push, ".enc_sec"))
extern "C" __declspec(code_seg(".enc_sec")) __declspec(noinline) void secureHello_body()
{
    volatile int _dummy = __COUNTER__;
    for(int i = 0; i < 1000; ++i) {
        _dummy += i; // Dummy operation to prevent optimization
    }
    // This function is protected by the license system
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::secureHello — Hello!"));
}
//__pragma(code_seg(pop))

__declspec(noinline) void secureHello()
{
    static volatile bool ready = false;
    static volatile int callCount = 0;
    callCount++;
    if (!ready) {
        lic::secure::antitamper::RuntimeCheck();
        if (callCount % 10 == 0) {
            if (lic::secure::antitamper::IsDebuggerPresent()) {
                for (int i = 0; i < 1000; i++) volatile auto x = rand();
                return;
            }
        }
        if (lic::LB_IsValid()) {
            lic::secure::FunctionGuard guard(reinterpret_cast<void*>(&secureHello_body));
            secureHello_body();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
            return;
        }
    } else {
        lic::secure::antitamper::AntiDebugCheck();
    }
}

// Create function log all sections in the module with name and address
__declspec(noinline)  void LogAllSections()
{
    // 1. Базовый адрес образа (для EXE ≈ ImageBase из PE)
    HMODULE hMod = nullptr;
    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&LogAllSections), &hMod))
    {
        UE_LOG(LogTemp, Warning, TEXT("GetModuleHandleEx failed"));
        return;
    }

    auto* Base = reinterpret_cast<uint8_t*>(hMod);
    //uint8_t* Base = reinterpret_cast<uint8_t*>(::GetModuleHandleW(nullptr));
    if (!Base) return;

	// Log the base address of the module
	UE_LOG(LogTemp, Log, TEXT("Base address: 0x%p"), Base);

    // 2. PE‑заголовки
    const auto* Dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(Base);
    if (Dos->e_magic != IMAGE_DOS_SIGNATURE) return;

    const auto* Nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        Base + Dos->e_lfanew);
    if (Nt->Signature != IMAGE_NT_SIGNATURE) return;

    const IMAGE_FILE_HEADER& FileHdr = Nt->FileHeader;
    const IMAGE_SECTION_HEADER* SecHdr = IMAGE_FIRST_SECTION(Nt);

    // 3. Перебираем секции
    for (WORD i = 0; i < FileHdr.NumberOfSections; ++i, ++SecHdr)
    {
        // PE‑имя секции – это char[8] без гарантированного '\0'
        char Name[9] = {};
        memcpy(Name, SecHdr->Name, 8);

        // Размер в памяти: берём VirtualSize, если он есть,
        // иначе SizeOfRawData (некоторые линкеры наоборот)
        size_t Size = SecHdr->Misc.VirtualSize ?
            SecHdr->Misc.VirtualSize : SecHdr->SizeOfRawData;

        const uint8_t* RVA = reinterpret_cast<const uint8_t*>(
            Base + SecHdr->VirtualAddress);

        UE_LOG(LogTemp, Log,
            TEXT("Section %2u: %-8S  RVA = 0x%08X, size = 0x%X (%u)"),
            i, Name,
            SecHdr->VirtualAddress,
            static_cast<uint32>(Size), static_cast<uint32>(Size));
    }
}

// Search section from LogAllSections and show name and address
void ShowSectionOfFunction(void* address)
{
    // 1. Базовый адрес образа (для EXE ≈ ImageBase из PE)
    HMODULE hMod = nullptr;
    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&ShowSectionOfFunction), &hMod))
    {
        UE_LOG(LogTemp, Warning, TEXT("GetModuleHandleEx failed"));
        return;
    }
    auto* Base = reinterpret_cast<uint8_t*>(hMod);
    if (!Base) return;
    // 2. PE‑заголовки
    const auto* Dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(Base);
    if (Dos->e_magic != IMAGE_DOS_SIGNATURE) return;
    const auto* Nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        Base + Dos->e_lfanew);
    if (Nt->Signature != IMAGE_NT_SIGNATURE) return;
    const IMAGE_FILE_HEADER& FileHdr = Nt->FileHeader;
    const IMAGE_SECTION_HEADER* SecHdr = IMAGE_FIRST_SECTION(Nt);
    // 3. Перебираем секции
    for (WORD i = 0; i < FileHdr.NumberOfSections; ++i, ++SecHdr)
    {
        size_t Size = SecHdr->Misc.VirtualSize ?
            SecHdr->Misc.VirtualSize : SecHdr->SizeOfRawData;
        const uint8_t* RVA = reinterpret_cast<const uint8_t*>(
            Base + SecHdr->VirtualAddress);
        if (reinterpret_cast<uint8_t*>(address) >= RVA &&
            reinterpret_cast<uint8_t*>(address) < RVA + Size)
        {
            char Name[9] = {};
            memcpy(Name, SecHdr->Name, 8);

			// Log the section name and address
            UE_LOG(LogTemp, Log,
                TEXT("Function %p found in section %2u: %-8S  RVA = 0x%08X, size = 0x%X (%u)"),
                address, i, Name,
                SecHdr->VirtualAddress,
				static_cast<uint32>(Size), static_cast<uint32>(Size));
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Function %p not found in any section"), address);
}
*/

void UMyGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::Init — Hello!"));

    LIC_GUARD_MAIN();

    //LogAllSections();
	//ShowSectionOfFunction(&secureHello_body);
    ProtectedFunction();
    //secureHello();
}

void UMyGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::Shutdown"));
    Super::Shutdown();
}

PROTECT_METHOD_IMPL(UMyGameInstance, void, ProtectedFunction, (), (),
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::ProtectedFunction — Hello!"));
    
    if (lic::LB_HasFeature("premium"))
    {
        features.Add("premium");
    }
})

/*
void UMyGameInstance::ProtectedFunction()
{
    UE_LOG(LogTemp, Log, TEXT("UTSGameInstance::ProtectedFunction — Hello!"));
}
*/

#pragma comment(linker, "/retain:.enc_Pro")
#pragma comment(linker, "/retain:.enc_sec")