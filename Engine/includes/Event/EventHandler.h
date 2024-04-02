#pragma once
#include <algorithm>
#include <functional>
#include <vector>



namespace lve
{
    template<typename... Args>
    class EventHandler
    {
    public:
        using Delegate = std::function<void(Args& _args)>;

        void operator+=(Delegate _callback)
        {
            AddListener(_callback);
        }

        void operator-=(Delegate _callback)
        {
            RemoveListener(_callback);
        }

        void AddListener(Delegate _callback) {
            callbacks.push_back(_callback);
        }

        void Trigger() const {
            if (isActive)
            {
                for (auto& callback : callbacks) {
                    callback();
                }
            }
        }

        void RemoveListener(Delegate _callback) {
            callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), _callback), callbacks.end());
        }

        void Enable() { isActive = true; }
        void Disable() { isActive = false; }

        void SetActive(const bool _state) { isActive = _state; }
        bool GetActive() const { return isActive; }

        void Clear() { callbacks.clear(); }

        ~Event()
        {
            Clear();
        }

    private:
        bool isActive = true;
        std::vector<Delegate> callbacks;

    };


}
