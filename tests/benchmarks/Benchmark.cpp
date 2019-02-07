//
// Created by magkanar on 2/6/19.
//

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <set>
#define CATCH_CONFIG_MAIN
//#include <catch2/catch.hpp>

#include <basalt/basalt.hpp>

using basalt::network_t;
using basalt::node_id_t;
using basalt::node_t;
using basalt::node_uid_t;
using basalt::node_uids_t;

namespace bbp {
    namespace in_silico {

/** \name node payloads
 ** \{
 */

        struct synapse_t {
            unsigned char version;
            uint32_t pre_gid;
            uint32_t post_gid;
            uint32_t nrn_idx;
            bool is_excitatory;
            float pre_x;
            float pre_y;
            float pre_z;
            float post_x;
            float post_y;
            float post_z;

            std::ostream &serialize(std::ostream &ostr) const {
                return ostr << version << pre_gid << post_gid << nrn_idx << is_excitatory << pre_x << pre_y
                            << pre_y << pre_z << post_x << post_y << post_z;
            }

            void deserialize(std::istream &istr) {
                istr >> version >> pre_gid >> post_gid >> nrn_idx >> is_excitatory >> pre_x >> pre_y >>
                     pre_y >> pre_z >> post_x >> post_y >> post_z;
            }
        };

        struct neuron_t {
            uint32_t gid;

            std::vector<uint32_t> astro_idx;
            std::vector<uint32_t> syn_idx;

        };

        struct astrocyte_t {
            uint32_t astrocyte_id;
            uint32_t microdomain_id;

            float soma_center_x;
            float soma_center_y;
            float soma_center_z;

            float soma_radius;

            std::string name;
            std::string mtype;
            std::string morphology_filename;

            std::vector<uint32_t> synapses_idx;
            std::vector<uint32_t> neurons_idx;
        };

        struct microdomain_t
        {
            uint32_t microdomain_id;
            uint32_t astrocyte_id;

            // potentially different than astrocyte actual neighbors
            // microdomains form a tesselation
            std::vector<uint32_t> neighbors;

            // mesh data
            std::vector<std::vector<float>> vertex_coordinates;
            std::vector<std::vector<uint32_t>> triangles;

            // geometric centroid, not the same as the morphology soma center
            float centroid_x;
            float centroid_y;
            float centroid_z;

            double area;
            double volume;

            // unsure if this is really essential. Meshes are created from vertex_coordinates and triangles
            std::string mesh_filename;

            // maybe it's better to access neurons from synapses indirectly?
            std::vector<uint32_t> neurons_idx;
            std::vector<uint32_t> synapses_idx;

        };

        struct segment_t {
            uint32_t section_id;
            uint32_t segment_id;
            uint8_t type;
            float x1;
            float y1;
            float z1;
            float r1;
            float x2;
            float y2;
            float z2;
            float r2;
            // FIXME
        };

        struct edge_astrocyte_segment_t {

            // endfoot starting point on morphology
            float astrocyte_x;
            float astrocyte_y;
            float astrocyte_z;

            // endfoot ending point on vasculature surface
            float vasculature_x;
            float vasculature_y;
            float vasculature_z;

        };

        struct edge_astrocyte_synapse_t {

            uint32_t morphology_neurite_id;

        };

        struct edge_synapse_neuron_t {
            // FIXME
        };

    }
}

/// \brief different types of node
enum node_type {none, neuron, synapse, astrocyte, microdomain, segment };

static void Read_Astr_Neur(std::string basalt_db){
    using basalt::network_t;
    using bbp::in_silico::synapse_t;

    network_t g(basalt_db);

    // iterate over all nodes
    std::set<node_uid_t> nodes_set;
    std::set<node_uid_t> astrocytes_set;
    int count = 0;
    for (const auto& node: g.nodes()) {
        std::cout << node << std::endl;
        ++count;
        nodes_set.insert(node);
        if(node.first==node_type::astrocyte) {
            astrocytes_set.insert(node);
        }
    }

    std::cout << "Neurons" << std::endl;
    for (const auto& astrocyte: astrocytes_set) {
        node_uids_t nodes;
        auto res_n = g.connections().get(astrocyte, node_type::neuron, nodes);
        for (auto const& node: nodes) {
            std::cout << astrocyte << " ↔ " << node << std::endl;
        }
    }
}

static void Read_Astr_Syn(std::string basalt_db){
    using basalt::network_t;
    using bbp::in_silico::synapse_t;

    network_t g(basalt_db);

    // iterate over all nodes
    std::set<node_uid_t> nodes_set;
    std::set<node_uid_t> astrocytes_set;
    int count = 0;
    for (const auto& node: g.nodes()) {
        std::cout << node << std::endl;
        ++count;
        nodes_set.insert(node);
        if(node.first==node_type::astrocyte) {
            astrocytes_set.insert(node);
        }
    }

    std::cout << "Synapses" << std::endl;
    for (const auto& astrocyte: astrocytes_set) {
        node_uids_t nodes;
        auto res_s = g.connections().get(astrocyte, node_type::synapse, nodes);
        for (auto const& node: nodes) {
            std::cout << astrocyte << " ↔ " << node << std::endl;
        }
    }
}

int main(void){
    /*using basalt::network_t;
    using bbp::in_silico::synapse_t;

    network_t g("/home/magkanar/basalt_benchmarking/basalt/build/tests/benchmarks/basalt-db_10");

    // iterate over all nodes
    std::set<node_uid_t> nodes_set;
    std::set<node_uid_t> astrocytes_set;
    int count = 0;
    for (const auto& node: g.nodes()) {
        std::cout << node << std::endl;
        ++count;
        nodes_set.insert(node);
        if(node.first==node_type::astrocyte) {
            astrocytes_set.insert(node);
        }
    }
//    std::cout << astrocyte << std::endl;
//    std::cout << "Astrocytes" << std::endl;
//    for (const auto& astrocyte: astrocytes_set) {
//        std::cout << astrocyte << std::endl;
//    }
    std::cout << "Neurons" << std::endl;
    for (const auto& astrocyte: astrocytes_set) {
        node_uids_t nodes;
        auto res_n = g.connections().get(astrocyte, node_type::neuron, nodes);
        for (auto const& node: nodes) {
            std::cout << astrocyte << " ↔ " << node << std::endl;
        }
    }

    std::cout << "Synapses" << std::endl;
    for (const auto& astrocyte: astrocytes_set) {
        node_uids_t nodes;
        auto res_s = g.connections().get(astrocyte, node_type::synapse, nodes);
        for (auto const& node: nodes) {
            std::cout << astrocyte << " ↔ " << node << std::endl;
        }
    }*/

    Read_Astr_Neur("/home/magkanar/basalt_benchmarking/basalt/build/tests/benchmarks/basalt-db_1");
    Read_Astr_Syn("/home/magkanar/basalt_benchmarking/basalt/build/tests/benchmarks/basalt-db_1");

}