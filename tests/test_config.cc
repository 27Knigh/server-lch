#include "lch/config.h"
#include "lch/log.h"
#include <yaml-cpp/yaml.h>

lch::ConfigVar<int>::ptr g_int_value_config = lch::Config::Lookup("system.port", 
                                                                (int)8080,
                                                                "system port");

lch::ConfigVar<float>::ptr g_float_value_config = lch::Config::Lookup("system.value", 
                                                                (float)10.2f,
                                                                "system value");
                                                                
lch::ConfigVar<std::vector<int> >::ptr g_int_vec_value_config = lch::Config::Lookup("system.int_vec", 
                                                                std::vector<int>{1, 2, 3},
                                                                "system int_vec");     
                                                                
lch::ConfigVar<std::list<int> >::ptr g_int_list_value_config = lch::Config::Lookup("system.int_list", 
                                                                std::list<int>{1, 2, 3},
                                                                "system int_list");     

lch::ConfigVar<std::set<int> >::ptr g_int_set_value_config = lch::Config::Lookup("system.int_set", 
                                                                std::set<int>{1, 2, 3},
                                                                "system int_set");    

lch::ConfigVar<std::unordered_set<int> >::ptr g_int_unordered_set_value_config = lch::Config::Lookup("system.int_unordered_set", 
                                                                std::unordered_set<int>{1, 2, 3, 3},
                                                                "system int_unordered_set");  

lch::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config = lch::Config::Lookup("system.str_int_map", 
                                                                std::map<std::string, int>{{"k", 1}, {"k2", 2}},
                                                                "system str_int_map");  

lch::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_umap_value_config = lch::Config::Lookup("system.str_int_umap", 
                                                                std::unordered_map<std::string, int>{{"k", 1}, {"k2", 2}},
                                                                "system str_int_umap");  


void print_yaml(const YAML::Node& node, int level) {
    if (node.IsScalar()) {
        LCH_LOG_INFO(LCH_LOG_ROOT()) << std::string(level * 4, ' ')
            << node.Scalar() << " - " << node.Type() << " - " << level;
    } else if (node.IsNull()) {
        LCH_LOG_INFO(LCH_LOG_ROOT()) << std::string(level * 4, ' ')
            << "NULL - " << node.Type() << " - " << level;
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++ it) {
            LCH_LOG_INFO(LCH_LOG_ROOT()) << std::string(level * 4, ' ')
                << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    } else if (node.IsSequence()) {
        for (size_t i = 0; i < node.size(); ++i) {
            LCH_LOG_INFO(LCH_LOG_ROOT()) << std::string(level * 4, ' ')
                << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml() {
    YAML::Node root = YAML::LoadFile("/home/lch/share/server-lch/bin/conf/log.yml");
    //LCH_LOG_INFO(LCH_LOG_ROOT()) << root;
    print_yaml(root, 0);
    LCH_LOG_INFO(LCH_LOG_ROOT()) << root.Scalar();
}

void test_config() {
    LCH_LOG_INFO(LCH_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    LCH_LOG_INFO(LCH_LOG_ROOT()) << "before: " << g_float_value_config->getValue();

#define XX(g_var, name, prefix) \
    do { \
        auto v = g_var->getValue(); \
        for (auto& i : v) { \
            LCH_LOG_INFO(LCH_LOG_ROOT()) << #prefix " " #name ": " << i; \
        } \
        LCH_LOG_INFO(LCH_LOG_ROOT()) << #prefix " " #name " yaml: \n" << g_var->toString(); \
    } while(0)

#define XX_M(g_var, name, prefix) \
    do { \
        auto v = g_var->getValue(); \
        for (auto& i : v) { \
            LCH_LOG_INFO(LCH_LOG_ROOT()) << #prefix " " #name ": {" << i.first << "," << i.second << "}"; \
        } \
        LCH_LOG_INFO(LCH_LOG_ROOT()) << #prefix " " #name " yaml: \n" << g_var->toString(); \
    } while(0)

    XX(g_int_vec_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_unordered_set_value_config, int_unordered_set, before);
    XX_M(g_str_int_map_value_config, str_int_map, before);
    XX_M(g_str_int_umap_value_config, str_int_umap, before);

    YAML::Node root = YAML::LoadFile("/home/lch/share/server-lch/bin/conf/log.yml");
    lch::Config::LoadYamlFile(root);

    LCH_LOG_INFO(LCH_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    LCH_LOG_INFO(LCH_LOG_ROOT()) << "after: " << g_float_value_config->getValue();

    XX(g_int_vec_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_unordered_set_value_config, int_unordered_set, after);
    XX_M(g_str_int_map_value_config, str_int_map, after);
    XX_M(g_str_int_umap_value_config, str_int_umap, after);
}

int main(int argc, char** argv) {

    test_config();
    //test_yaml();

    return 0;
}