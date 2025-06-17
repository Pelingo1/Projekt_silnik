#pragma once
#include <string>
#include "../Context.h"

namespace RayEngine {
    class Entity;
    class Component {
    public:
        virtual ~Component() = default;
        virtual const std::string &GetID() const = 0;
        Entity *GetOwner() const {return m_Owner;}

        virtual void Init(Entity *_owner) {
            m_Owner = _owner;
        }

        virtual Component *Clone() const = 0;

        virtual void OnStart() {

        }
        virtual void OnUpdate(const UpdateContext &context) {

        }
        virtual void OnRender(const RenderContext &context) const {

        }
        virtual void OnRenderUI(const RenderUIContext &context) const {

        }
        virtual void OnDestroy() {

        }

    private:
        Entity *m_Owner = nullptr;

    };
}

