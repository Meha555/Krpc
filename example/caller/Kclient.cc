#include "../user.pb.h"
#include "Krpcchannel.h"
#include "Krpcconfig.h"
#include "Krpccontroller.h"
#include <atomic>
#include <chrono>
#include <glog/logging.h>
#include <thread>

static void send_request(int thread_id, std::atomic<int> &success_count, std::atomic<int> &fail_count)
{
    Kuser::UserServiceRpc_Stub stub(new KrpcChannel());

    // rpc方法请求参数
    Kuser::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");

    // rpc方法响应参数
    Kuser::LoginResponse response;
    KrpcController controller;
    stub.Login(&controller, &request, &response, nullptr); // 父类Rpchannel->子类Rpchannel::callmethod
    // 读调用的结果
    if (controller.Failed()) {
        LOG(ERROR) << controller.ErrorText();
    } else {
        if (0 == response.result().errcode()) {
            LOG(INFO) << "rpc login response success: " << response.success();
            success_count++;
        } else {
            LOG(ERROR) << "rpc login response error: " << response.result().errmsg();
            fail_count++;
        }
    }
}

int main(int argc, char **argv)
{
    KrpcConfig::InitEnv(argc, argv);

    const int thread_count = std::thread::hardware_concurrency(); // 并发线程数
    const int requests_per_thread = 10; // 每个线程发送的请求数

    std::vector<std::thread> threads;
    std::atomic<int> success_count(0);
    std::atomic<int> fail_count(0);

    auto start_time = std::chrono::high_resolution_clock::now(); // 开始时间
    // 启动多线程进行并发测试
    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back([i, &success_count, &fail_count]() {
            for (int j = 0; j < requests_per_thread; j++) {
                send_request(i, success_count, fail_count);
            }
        });
    }
    for (auto &t : threads) {
        t.join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 输出统计结果
    LOG(INFO) << "==================================";
    LOG(INFO) << "Total requests: " << thread_count * requests_per_thread;
    LOG(INFO) << "Success count: " << success_count;
    LOG(INFO) << "Fail count: " << fail_count;
    LOG(INFO) << "Elapsed time: " << elapsed.count() << " seconds";
    LOG(INFO) << "QPS: " << (thread_count * requests_per_thread) / elapsed.count();
    return 0;
}
