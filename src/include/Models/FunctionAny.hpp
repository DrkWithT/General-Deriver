#pragma once

#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <memory>
#include <utility>

#include "Models/IFunction.hpp"

namespace GeneralDeriver::Models {
    /**
     * @brief Strips cv-qualifiers & reference marks from a type. Used especially for FunctionAny!
     * @tparam Tp 
     */
    template <typename Tp>
    using naked_t = std::remove_reference_t<std::remove_cv_t<Tp>>;

    /**
     * @brief Homemade, type-erasure based container for any algebraic function instance.
     * @note Caveats: will rely a bit much on the heap. Copy operations are not implemented yet.
     */
    class FunctionAny {
    private:
        struct IStorage {
            virtual ~IStorage() = default;
            virtual IFunction* getIPointer() = 0;
            virtual bool isEmpty() const = 0;
            virtual const std::type_info& getTypeInfo() const = 0;
        };

        template <typename Tp> requires (std::is_base_of_v<IFunction, Tp>)
        struct Storage : public IStorage {
            using plain_func_t = naked_t<Tp>;
            std::unique_ptr<IFunction> ptr;

            Storage(Tp& func) : ptr(std::make_unique<Tp>(func)) {}

            Storage(Tp&& x_func) : ptr(std::make_unique<Tp>(x_func)) {}

            IFunction* getIPointer() override {
                return ptr.get();
            }

            [[nodiscard]] bool isEmpty() const override {
                return ptr == nullptr;
            }

            const std::type_info& getTypeInfo() const override {
                return typeid(plain_func_t);
            }
        };

        std::unique_ptr<IStorage> storage_ptr;

        /* private helper methods */

        [[nodiscard]] bool hasItem() const { return storage_ptr != nullptr; }

        template <typename Tp>
        [[nodiscard]] bool hasItemOfType() const {
            if (!hasItem()) {
                return false;
            }

            return typeid(naked_t<Tp>) == storage_ptr->getTypeInfo();
        }

        void swapWith(FunctionAny&& x_other) noexcept {
            this->storage_ptr.swap(x_other.storage_ptr);
        }

    public:
        constexpr FunctionAny() : storage_ptr {nullptr} {}

        template <typename Tp>
        FunctionAny(Tp&& any_func) : storage_ptr(std::make_unique<Storage<Tp>>(any_func)) {}

        FunctionAny(const FunctionAny& other) = delete;
        FunctionAny& operator=(const FunctionAny& other) = delete;

        FunctionAny(FunctionAny&& x_other) noexcept {
            this->swapWith(std::move(x_other));
        }

        FunctionAny& operator=(FunctionAny&& x_other) noexcept {
            this->swapWith(std::move(x_other));
            return *this;
        }

        template <typename FuncTp>
        auto unpackFunctionAny() -> naked_t<FuncTp> {
            if (!hasItemOfType<FuncTp>()) {
                throw std::runtime_error {"FunctionAny::AccessError: Invalid unpack type passed to unpackFunctionAny!"};
            }

            const FuncTp* result_ptr = dynamic_cast<const FuncTp*>(storage_ptr->getIPointer());

            return *result_ptr;
        }
    };
}