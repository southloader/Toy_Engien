#pragma once

#include "SampleCombatRequest.h"
#include "SampleCombatResult.h"

class SampleCombatSession {
public:
    bool Begin(const SampleCombatRequest& request) {
        if (!request.IsValid()){
            return false;
        }

        currentRequest = request;
        currentResult = SampleCombatResult();

        active = true;
        resultReady = false;

        return true;
    }
    void Finish(const SampleCombatResult& result) {
        if (!active){
            return;
        }

        currentResult = result;

        active = false;
        resultReady = true;
    }
    void Cancel() {
        currentRequest = SampleCombatRequest();
        currentResult = SampleCombatResult();

        active = false;
        resultReady = false;
    }
    bool IsActive() const {
        return active;
    }
    bool HasResult() const {
        return resultReady;
    }

    const SampleCombatRequest& GetRequest() const {
        return currentRequest;
    }
    const SampleCombatResult& GetResult() const {
        return currentResult;
    }
    
    void ClearResult() {
        currentResult = SampleCombatResult();
        resultReady = false;
    }

private:
    SampleCombatRequest currentRequest;
    SampleCombatResult currentResult;

    bool active = false;
    bool resultReady = false;
};