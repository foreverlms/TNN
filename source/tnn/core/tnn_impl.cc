// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "tnn/core/tnn_impl.h"

#include "tnn/interpreter/net_structure.h"

namespace TNN_NS {

TNNImpl::~TNNImpl() {
    DeInit();
}

Status TNNImpl::DeInit() {
    return TNN_OK;
}

Status TNNImpl::Init(ModelConfig &config) {
    model_config_ = config;
    return TNN_OK;
}

std::map<ModelType, std::shared_ptr<AbstractTNNImplFactory>> &TNNImplManager::GetTNNImplFactoryMap() {
    //这个map由于是静态的，因此一直存在于内存
    static std::map<ModelType, std::shared_ptr<AbstractTNNImplFactory>> s_tnn_impl_factory_map;
    return s_tnn_impl_factory_map;
}

std::shared_ptr<TNNImpl> TNNImplManager::GetTNNImpl(ModelType type) {
    auto &impl_map = TNNImplManager::GetTNNImplFactoryMap();
    auto iter      = impl_map.find(type);
    //判断输入的模型tnn是否支持，只支持ncnn与tnn格式
    //注册已在tnn_impl_default.cc里实现，因为是全局变量？
    if (iter != impl_map.end()) {
        return iter->second->CreateTNNImp();
    }

    return nullptr;
}

void TNNImplManager::RegisterTNNImplFactory(ModelType type, AbstractTNNImplFactory *factory) {
    if (factory) {
        //向s_tnn_impl_factory_map这个map里注册模型类别
        auto &optimizer_map = TNNImplManager::GetTNNImplFactoryMap();
        optimizer_map[type] = std::shared_ptr<AbstractTNNImplFactory>(factory);
    }
}

}  // namespace TNN_NS
