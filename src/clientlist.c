#include "redismodule.h"

static const char* CLIENT_KEY_NAME = "clientlist";

void ClientList_EventCallBack(RedisModuleCtx *ctx, RedisModuleEvent eid, uint64_t subevent, void *data) {
    RedisModuleClientInfo *client_info = (RedisModuleClientInfo *) data;

    if (subevent == REDISMODULE_SUBEVENT_CLIENT_CHANGE_CONNECTED) {
        RedisModule_Log(ctx, REDISMODULE_LOGLEVEL_NOTICE, "Connected: %s:%d (clientId=%lld, flags=%lld)\n", client_info->addr, client_info->port, client_info->id, client_info->flags);
        RedisModuleCallReply* reply = RedisModule_Call(ctx, "SADD", "cc", CLIENT_KEY_NAME, client_info->addr);
        RedisModule_FreeCallReply(reply);
    } else if (subevent == REDISMODULE_SUBEVENT_CLIENT_CHANGE_DISCONNECTED) {
        RedisModule_Log(ctx, REDISMODULE_LOGLEVEL_NOTICE, "Disconnected: %s:%d(clientId=%lld, flags=%lld)\n", client_info->addr, client_info->port, client_info->id, client_info->flags);
    }
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx,"clientlist",1,REDISMODULE_APIVER_1)
        == REDISMODULE_ERR) return REDISMODULE_ERR;

    RedisModuleEventCallback callback = ClientList_EventCallBack;
    return RedisModule_SubscribeToServerEvent(ctx, RedisModuleEvent_ClientChange, callback);
}
