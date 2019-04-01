#include <basalt/edge_iterator.hpp>
#include <basalt/edges.hpp>

#include "edge_iterator_impl.hpp"
#include "graph_impl.hpp"

namespace basalt {
EdgeIterator::EdgeIterator(const basalt::GraphImpl& pimpl, size_t from) {
    if (from == std::numeric_limits<std::size_t>::max()) {
        pimpl_ = EdgeIteratorImpl_ptr(nullptr);
    } else {
        pimpl_ = pimpl.edge_iterator(from);
        std::advance(*this, static_cast<EdgeIterator::difference_type>(from));
    }
}

EdgeIterator::EdgeIterator(const basalt::EdgeIterator& other)
    : pimpl_(other.pimpl_) {}

EdgeIterator& EdgeIterator::operator++() {
    ++*pimpl_;
    return *this;
}

const EdgeIterator EdgeIterator::operator++(int value) {
    const EdgeIterator result(*this);
    while (value-- > 0) {
        this->operator++();
    }
    return result;
}

bool EdgeIterator::operator==(const basalt::EdgeIterator& other) const {
    if (pimpl_) {
        if (other.pimpl_) {
            return *this->pimpl_ == *other.pimpl_;
        }
        return pimpl_->end_reached();
    }
    return !other.pimpl_;
}

bool EdgeIterator::operator!=(const basalt::EdgeIterator& other) const {
    return !(*this == other);
}

const EdgeIterator::value_type& EdgeIterator::operator*() {
    return **pimpl_;
}

}  // namespace basalt