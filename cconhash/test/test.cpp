#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "conhash.h"
#include <string>
#include <map>
#include <iostream>
#include <thread>

class Conhash
{
public:

    Conhash() 
    {
        _conhash = conhash_init(NULL);
        assert(NULL!=_conhash);
        
        _nodemaps.clear();
    }

    ~Conhash() 
    {
        std::map<std::string, void*>::iterator it;
        for (it = _nodemaps.begin(); it != _nodemaps.end(); ++it) 
        {
            struct node_s *tmp = (struct node_s *) (it->second);
            assert(NULL!=tmp);
            delete tmp;
        }
        
        _nodemaps.clear();
        
        if (NULL != _conhash) 
        {
            conhash_fini(_conhash);
            _conhash = NULL;
        } 
    }
    
    void add_node(const char *iden,int index,const int replica)
    {
        std::map<std::string, void*>::iterator it;
        it = _nodemaps.find(iden);
        if (_nodemaps.end() == it)
        {
            struct node_s *node = new(std::nothrow) struct node_s;
            assert(NULL != node);

            conhash_set_node(node, index, iden, replica);
            conhash_add_node(_conhash, node);
            _nodemaps[iden] = node;
        }
    }
    
    void delete_node(const char *iden)
    {
        std::map<std::string,void*>::iterator it;
        it=_nodemaps.find(iden);
        if(_nodemaps.end()!=it)
        {
            struct node_s *node=(struct node_s *)(it->second);
            conhash_del_node(_conhash, node);
            delete node;
            node=NULL;
            
            _nodemaps.erase(it);
        }
    }
    
    const char* lookup_node(const char *keystr)
    {
        const struct node_s *node = conhash_lookup(_conhash, keystr, strlen(keystr));
        if (NULL != node) 
        {
            std::map<std::string, void*>::iterator it;
            for (it = _nodemaps.begin(); it != _nodemaps.end(); ++it) 
            {
                struct node_s *tmp = (struct node_s *) (it->second);
                if (tmp == node)
                    return it->first.c_str();
            }
        }
        
        return NULL;
    }
    
    
    inline const int vnodes_num()
    {
        return conhash_get_vnodes_num(_conhash);
    }
    
    inline const int rnodes_num()
    {
        return (int)_nodemaps.size();
    }

protected:
    Conhash(const Conhash&);
    Conhash& operator=(const Conhash&);
    
private:
    struct conhash_s * _conhash;
    std::map<std::string,void*> _nodemaps;
};

void threadFunc(const std::string &s, conhash_s *conhash)
{
    char str[128];
     for (int i = 1; i <= 30000; i++)
    {
         sprintf(str, "%s,James.km%03d", s.c_str(), i);
        const struct node_s *node = conhash_lookup(conhash, str, strlen(str));
        if(node)
            printf("[%16s] is in node: [%16s]\n", str, node->iden);
        //std::cout<<"["<<s<<"] is in node: ["<<rnode<<"]"<<std::endl;
    }
}

struct node_s g_nodes[64];
int main(int argc,char *argv[])
{
  int i;
    const struct node_s *node;
    char str[128];
    long hashes[512];

    /* init conhash instance */
    struct conhash_s *conhash = conhash_init(NULL);
    if(conhash)
    {
        /* set nodes */
        conhash_set_node(&g_nodes[0],0, "titanic", 1000);
        conhash_set_node(&g_nodes[1],1, "terminator2018", 2000);
        conhash_set_node(&g_nodes[2],2, "Xenomorph", 4000);
        conhash_set_node(&g_nodes[3],3, "True Lies", 1000);
        conhash_set_node(&g_nodes[4],4, "avantar", 2000);

        /* add nodes */
        conhash_add_node(conhash, &g_nodes[0]);
        conhash_add_node(conhash, &g_nodes[1]);
        conhash_add_node(conhash, &g_nodes[2]);
        conhash_add_node(conhash, &g_nodes[3]);
        conhash_add_node(conhash, &g_nodes[4]);

        printf("virtual nodes number %d\n", conhash_get_vnodes_num(conhash));
        printf("the hashing results--------------------------------------:\n");
     }

     std::thread ta(threadFunc, "ta", conhash);
     std::thread tb(threadFunc, "tb", conhash);
     std::thread tc(threadFunc, "tc", conhash);
     std::thread td(threadFunc, "td", conhash);
     std::thread te(threadFunc, "te", conhash);
     std::thread tf(threadFunc, "tf", conhash);

     ta.join();
     tb.join();
     tc.join();
     td.join();
     te.join();
     tf.join();
     return 0;
}

/*
int main(int argc,char *argv[])
{
    Conhash hash;
    hash.add_node("192.168.100.1;10091", 0, 50);
    hash.add_node("192.168.100.2;10091", 1, 50);
    hash.add_node("192.168.100.3;10091", 2, 50);
    hash.add_node("192.168.100.4;10091", 3, 50);

    std::thread ta(threadFunc, "ta", &hash);
    std::thread tb(threadFunc, "tb", &hash);

    ta.join();
    tb.join();

    std::cout << "virtual nodes number :" << hash.vnodes_num() << std::endl;
    std::cout << "real nodes number :" << hash.rnodes_num() << std::endl;
    std::cout << "the hashing results------------------:" << std::endl;
    
    return 0;
}
*/