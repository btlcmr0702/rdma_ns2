//by btl
#include "agent.h"  
#include "tclcl.h"  
#include "packet.h"  
#include "address.h"  
#include "ip.h"  

#define SYN 1
#define SYN_ACK 2
#define ACK 3

struct hdr_rdmauc {  
    double ts_;  
    int type_;  
    int seqno_;
    int prior_;  
    static int offset_;  
    inline static int& offset() {  
        return offset_;  
    }  
    inline static hdr_rdmauc * access(const Packet * p) {  
        return (hdr_rdmauc*) p->access(offset_);  
    }     
    	int& seqno() { return (seqno_); }
   	double& ts() { return (ts_); }
        int& type() { return (type_); }
        int& prior() { return (prior_); }
};  

  
class rdmauc_agent : public Agent {  
      
    public :  
        rdmauc_agent();  
        virtual void recv(Packet *, Handler *);  
        void send_msg(int seqno); //创建并发送数据  
       // virtual void sendmsg(int nbytes, const char *flags = 0); 
          
    protected:            
        int command(int argc, const char*const*argv);  
        int t_seqno;
        int curseq;
};  
  
