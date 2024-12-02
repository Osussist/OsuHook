#include "ipc.h"

ICLRMetaHost* IPC::pMetaHost = nullptr;
ICLRRuntimeInfo* IPC::pRuntimeInfo = nullptr;
ICLRRuntimeHost* IPC::pRuntimeHost = nullptr;
std::string IPC::assemblyPath;

IPC::IPC(Logger sdkLogger, HANDLE processHandle) : logger(sdkLogger) {
    char buffer[MAX_PATH];
    GetModuleFileNameExA(processHandle, NULL, buffer, MAX_PATH);
    IPC::assemblyPath = buffer;
    HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    if (FAILED(hr)) {
        logger.error(__FUNCTION__, "Failed to create CLR instance");
        return;
    }
	logger.debug(__FUNCTION__, "CLR instance created");
    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
    if (FAILED(hr)) {
        logger.error(__FUNCTION__, "Failed to get runtime info");
        return;
    }
	logger.info(__FUNCTION__, "Runtime info obtained");
    hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&pRuntimeHost));
    if (FAILED(hr)) {
        logger.error(__FUNCTION__, "Failed to get runtime host");
        pRuntimeInfo->Release();
        pMetaHost->Release();
        return;
    }
	logger.debug(__FUNCTION__, "Runtime host obtained");
	hr = pRuntimeHost->Start();
	if (FAILED(hr)) {
		logger.error(__FUNCTION__, "Failed to start runtime host");
		pRuntimeHost->Release();
		pRuntimeInfo->Release();
		pMetaHost->Release();
		return;
	}
	logger.info(__FUNCTION__, "IPC initialized successfully");
}

ClientData IPC::get_client_data() {
    HRESULT hr;
    DWORD pReturnValue;
    DWORD appDomainId;
    hr = pRuntimeHost->GetCurrentAppDomainId(&appDomainId);
    if (FAILED(hr)) {
        logger.error(__FUNCTION__, "Failed to get default AppDomain");
        return ClientData();
    }
	//hr = pRuntimeHost->ExecuteInAppDomain(appDomainId);
    if (FAILED(hr)) {
        logger.error(__FUNCTION__, "Failed to execute method");
        return ClientData();
    }

    ClientData* pClientData = reinterpret_cast<ClientData*>(pReturnValue);
    ClientData clientData = *pClientData;

    return clientData;
}

// TODO: Fix later (this shit mad annoying)