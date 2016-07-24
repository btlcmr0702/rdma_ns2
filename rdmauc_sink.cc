#include "rdmauc_sink.h" 
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include "ip.h"
#include "flags.h"
#include "random.h"
#include "basetrace.h"
#include "hdr_qs.h"
 
//显示系统时间  
#define NOW Scheduler::instance().clock()  
//得到当前节点的地址  
#define MYNODE Address::instance().get_nodeaddr(addr()) 
 
int hdr_rdmauc_sink::offset_;  
  
static class rdmauc_sinkHeaderClass : public PacketHeaderClass {  
    public:  
         rdmauc_sinkHeaderClass() : PacketHeaderClass("PacketHeader/rdmauc_sink",sizeof(hdr_rdmauc_sink)) {  
            bind_offset(&hdr_rdmauc_sink::offset_);  
        }  
} class_rdmauc_sinkdr;  
  
static class rdmauc_sinkAgentClass : public TclClass {  
public:  
    rdmauc_sinkAgentClass() : TclClass("Agent/RDMAUC_SINK") {}  
    TclObject* create(int, const char*const*) {  
        return (new rdmauc_sink_agent());  
    }  
} class_rdmauc_sink;  
  
rdmauc_sink_agent::rdmauc_sink_agent() : Agent(PT_RDMAUC_SINK),
           t_seqno(0),curseq(0)   
{  
     printf("sink has created!\n");  
    bind("t_seqno_", &t_seqno);  
    bind("curseq_", &curseq);  
}  


void rdmauc_sink_agent::send_msg(int seqno) {  
    //数据包的生成  
    Packet* pkt = allocpkt(); 
    //数据包的访问          
    hdr_ip *iph = hdr_ip::access(pkt);  
    hdr_rdmauc_sink *rdmauc_sinkdr = hdr_rdmauc_sink::access(pkt);  
    int databytes = hdr_cmn::access(pkt)->size();  

	
}  



void rdmauc_sink_agent::recv(Packet *p, Handler *) {  
      
    printf("test_secv!!!!!!!!!!");
    hdr_ip *iph = hdr_ip::access(p);      
    hdr_rdmauc_sink *rdmauc_sinkdr = hdr_rdmauc_sink::access(p);  
      
    if( rdmauc_sinkdr->type() == SYN ) {  
          
        printf("=>SYN SIGNAL At %lf node %d receive PROTOCOL_INIT_test signaling from node %d \n",   
                    rdmauc_sinkdr->ts(), MYNODE, iph->saddr());  
        printf("the port is : %d \n",iph->dport());
        
        Packet* pkt =allocpkt();
      
        hdr_ip *iph_send = hdr_ip::access(pkt);  
        hdr_rdmauc_sink *rdmauc_sinkdr_send = hdr_rdmauc_sink::access(pkt);  
        int databytes = hdr_cmn::access(pkt)->size();  

	iph_send->daddr()=iph->saddr();
        iph_send->dport()=iph->sport();
        rdmauc_sinkdr_send->type() = SYN_ACK;
        rdmauc_sinkdr_send->seqno() = t_seqno;
        rdmauc_sinkdr_send->ts() = NOW;
        
        send(pkt,0);
        t_seqno++;
        curseq++;  
    } else {  
          
         
    }  
}  


int rdmauc_sink_agent::command(int argc, const char*const* argv) {  
  /*
    if( argc == 2 ) {  
        if( !strcmp( argv[1], "begin" ) ) {       
  
            //sendsend_simple_msg( PROTOCOL_INIT_SYN, simple_target );  
            send_msg(t_seqno);  
             
            return (TCL_OK);          
        }  
    }     
     */                 
    
                  
    return (Agent::command(argc, argv));  
}  



