////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#include "impl/weak_realm_notifier.hpp"

#include "shared_realm.hpp"
#include "util/event_loop_signal.hpp"

using namespace realm;
using namespace realm::_impl;


WeakRealmNotifier::WeakRealmNotifier(const std::shared_ptr<Realm>& realm, bool cache)
: m_realm(realm)
, m_execution_context(realm->config().execution_context)
, m_realm_key(realm.get())
, m_cache(cache)
, m_signal(std::make_shared<util::EventLoopSignal<Callback>>(Callback{realm}))
{
}

WeakRealmNotifier::~WeakRealmNotifier() = default;

void WeakRealmNotifier::Callback::operator()() const
{
    if (auto realm = weak_realm.lock()) {
        realm->notify();
    }
}

void WeakRealmNotifier::notify()
{
    m_signal->notify();
}
