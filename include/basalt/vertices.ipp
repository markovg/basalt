#pragma once

#include <sstream>

#include <basalt/graph.hpp>
#include <basalt/status.hpp>

namespace basalt {

template <EdgeOrientation Orientation>
template <typename Payload>
Status Vertices<Orientation>::insert(const vertex_uid_t& vertex, const Payload& data, bool commit) {
    std::ostringstream oss;
    data.serialize(oss);
    // \fixme TCL get buffer beginning and length from ostringstream
    // to avoid extra std::string copy.
    const std::string raw(oss.str());
    return insert(vertex, raw.c_str(), raw.size(), commit);
}

template <EdgeOrientation Orientation>
template <typename T>
Status Vertices<Orientation>::get(const vertex_uid_t& vertex, T& payload) const {
    std::string data;
    auto const& status = get(vertex, &data);
    if (status) {
        std::istringstream istr;
        istr.rdbuf()->pubsetbuf(const_cast<char*>(data.c_str()), static_cast<long>(data.size()));
        payload.deserialize(istr);
    }
    return status;
}

}  // namespace basalt
