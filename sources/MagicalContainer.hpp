#ifndef MAGICALCONTAINER_HPP
#define MAGICALCONTAINER_HPP
#include <iostream>
#include <vector>
#include <list>
using namespace std;
namespace ariel
{

    class MagicalContainer
    {
    private:
        list<int> elements_container;
        vector<int *> prime_container;
        vector<int *> asc_container;
        vector<int *> cross_container;

        // helper function
        bool isPrime(int num);
        void updateIterContainer(int *elementPtr);
        void updatePrimeContainer(int *elementPtr);
        void updateAscContainer(int *elementPtr);
        void updateCrossContainer();

    public:
        void addElement(int element);
        void removeElement(int element);
        size_t size() const;

        // Iterator interface
        class CustomIterator
        {
        public:
            // ctor and dtor
            CustomIterator() = default;
            virtual ~CustomIterator() = default;
            // copy ctor and copy assignment
            CustomIterator(const CustomIterator &other) = default;
            CustomIterator &operator=(const CustomIterator &other) = default;
            // move ctor and move assignment for tidy
            CustomIterator(CustomIterator &&other) = default;
            CustomIterator &operator=(CustomIterator &&other) = default;

            // operatorss
            virtual int operator*() const = 0;
            virtual CustomIterator &operator++() = 0;
            virtual bool operator<(const CustomIterator &other) const = 0;
            virtual bool operator>(const CustomIterator &other) const = 0;
            virtual bool operator==(const CustomIterator &other) const = 0;
            virtual bool operator!=(const CustomIterator &other) const = 0;
        };
        // AscendingIterator
        class AscendingIterator : public CustomIterator
        {
        private:
            MagicalContainer &container;
            size_t index;
            AscendingIterator(MagicalContainer &container, size_t index); // private ctor for begin() and end()

        public:
            // ctor dtor and copy ctor
            AscendingIterator(MagicalContainer &container);
            ~AscendingIterator() override;
            AscendingIterator(const AscendingIterator &other);
            AscendingIterator &operator=(const AscendingIterator &other);
            // move ctor and move assignment for tidy
            AscendingIterator(AscendingIterator &&other) noexcept;
            AscendingIterator &operator=(AscendingIterator &&other) noexcept;
            // operators
            int operator*() const override;
            AscendingIterator &operator++() override;
            bool operator<(const CustomIterator &other) const override;
            bool operator>(const CustomIterator &other) const override;
            bool operator==(const CustomIterator &other) const override;
            bool operator!=(const CustomIterator &other) const override;
            // operators with AscendingIterator argument so it wont be ambiguous
            bool operator<(const AscendingIterator &other) const;
            bool operator>(const AscendingIterator &other) const;
            bool operator==(const AscendingIterator &other) const;
            bool operator!=(const AscendingIterator &other) const;
            AscendingIterator begin();
            AscendingIterator end();
        };
        // SideCrossIterator
        class SideCrossIterator : public CustomIterator
        {
        private:
            MagicalContainer &container;
            size_t index;
            SideCrossIterator(MagicalContainer &container, size_t index); // private ctor for begin() and end()

        public:
            // ctor dtor and copy ctor

            SideCrossIterator(MagicalContainer &container);
            ~SideCrossIterator() override;
            SideCrossIterator(const SideCrossIterator &other);
            SideCrossIterator &operator=(const SideCrossIterator &other);
            // move ctor and move assignment for tidy
            SideCrossIterator(SideCrossIterator &&other) noexcept;
            SideCrossIterator &operator=(SideCrossIterator &&other) noexcept;
            // operators
            int operator*() const override;
            SideCrossIterator &operator++() override;
            bool operator<(const CustomIterator &other) const override;
            bool operator>(const CustomIterator &other) const override;
            bool operator==(const CustomIterator &other) const override;
            bool operator!=(const CustomIterator &other) const override;
            // operators with SideCrossIterator argument so it wont be ambiguous
            bool operator<(const SideCrossIterator &other) const;
            bool operator>(const SideCrossIterator &other) const;
            bool operator==(const SideCrossIterator &other) const;
            bool operator!=(const SideCrossIterator &other) const;
            SideCrossIterator begin();
            SideCrossIterator end();
        };
        // PrimeIterator
        class PrimeIterator : public CustomIterator
        {
        private:
            MagicalContainer &container;
            size_t index;
            PrimeIterator(MagicalContainer &container, size_t index); // private ctor for begin() and end()

        public:
            // ctor dtor and copy ctor
            PrimeIterator(MagicalContainer &container);
            ~PrimeIterator() override;
            PrimeIterator(const PrimeIterator &other);
            PrimeIterator &operator=(const PrimeIterator &other);
            // move ctor and move assignment for tidy
            PrimeIterator(PrimeIterator &&other) noexcept;
            PrimeIterator &operator=(PrimeIterator &&other) noexcept;
            // operators
            int operator*() const override;
            PrimeIterator &operator++() override;
            bool operator<(const CustomIterator &other) const override;
            bool operator>(const CustomIterator &other) const override;
            bool operator==(const CustomIterator &other) const override;
            bool operator!=(const CustomIterator &other) const override;
            // operators with PrimeIterator argument so it wont be ambiguous
            bool operator<(const PrimeIterator &other) const;
            bool operator>(const PrimeIterator &other) const;
            bool operator==(const PrimeIterator &other) const;
            bool operator!=(const PrimeIterator &other) const;
            PrimeIterator begin();
            PrimeIterator end();
        };
    };
}

#endif