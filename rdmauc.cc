#include "rdmauc.h" 
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
 
int hdr_rdmauc::offset_;  
  
static class rdmaucHeaderClass : public PacketHeaderClass {  
    public:  
         rdmaucHeaderClass() : PacketHeaderClass("PacketHeader/rdmauc",sizeof(hdr_rdmauc)) {  
            bind_offset(&hdr_rdmauc::offset_);  
        }  
} class_rdmaucdr;  
  
static class rdmaucAgentClass : public TclClass {  
public:  
    rdmaucAgentClass() : TclClass("Agent/RDMAUC") {}  
    TclObject* create(int, const char*const*) {  
        return (new rdmauc_agent());  
    }  
} class_rdmauc;  
  
rdmauc_agent::rdmauc_agent() : Agent(PT_RDMAUC),
           t_seqno(0),curseq(0)   
{  
       
    bind("t_seqno_", &t_seqno);  
    bind("curseq_", &curseq);  
}  


void rdmauc_agent::send_msg(int seqno) {  

    printf("test_send!!!!!!!!\nseqno:%d , t_seqno: %d\n",seqno,t_seqno);
    //数据包的生成  
    Packet* pkt = allocpkt(); 
    //数据包的访问          
    hdr_ip *iph = hdr_ip::access(pkt);  
    hdr_rdmauc *rdmaucdr = hdr_rdmauc::access(pkt);  
    int databytes = hdr_cmn::access(pkt)->size();  

	if(t_seqno ==0 ){   //connect_init

        rdmaucdr->type() = SYN;
        rdmaucdr->seqno() = seqno;
        rdmaucdr->ts() = NOW;
        target_->recv(pkt);
        send(pkt,0); 
        t_seqno++;
        curseq++;
        printf("t_seqno: %d\n",t_seqno);
      }       
      else{
         

      }
       
      
}  



void rdmauc_agent::recv(Packet *p, Handler *) {  
      
    hdr_ip *iph = hdr_ip::access(p);      
    hdr_rdmauc *rdmaucdr = hdr_rdmauc::access(p);  
      
    if( rdmaucdr->type() == SYN_ACK ) {  
          
        printf("=>SYN_ACk SIGNAL At %lf node %d receive PROTOCOL_INIT_test signaling from node %d \n",   
                    rdmaucdr->ts(), MYNODE, iph->saddr());  
        printf("the port is : %d \n",iph->dport());
        
        Packet* pkt =allocpkt();
      
        hdr_ip *iph_send = hdr_ip::access(pkt);  
        hdr_rdmauc *rdmaucdr_send = hdr_rdmauc::access(pkt);  
        int databytes = hdr_cmn::access(pkt)->size();  

	iph_send->daddr()=iph->saddr();
        iph_send->dport()=iph->sport();
        rdmaucdr_send->type() = ACK;
        rdmaucdr_send->seqno() = t_seqno;
        rdmaucdr_send->ts() = NOW;
        
        send(pkt,0);
        t_seqno++;  
        curseq++;
    } else {  
          
         
    }  
}  


int rdmauc_agent::command(int argc, const char*const* argv) {  
  
    if( argc == 2 ) {  
        if( !strcmp( argv[1], "begin" ) ) {       
  
            //sendsend_simple_msg( PROTOCOL_INIT_SYN, simple_target );  
            send_msg(t_seqno);  
             
            return (TCL_OK);          
        }  
    }     
                      
    
                  
    return (Agent::command(argc, argv));  
}  



