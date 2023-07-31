package conhash

// #cgo CFLAGS: -I${SRCDIR}/
// #cgo LDFLAGS: -L${SRCDIR}/../libs -lconhash
// #include "../cbaseclass/cconhash/configure.h"
// #include "../cbaseclass/cconhash/conhash.h"
import "C"
import (
	"fmt"
	"strconv"
	"sync"
	"unsafe"
)

type ConHashNode_t struct {
	Iden     [64]int8
	Replicas uint32
	Index    uint32
	Flag     uint32
}

func main_test() {
	g_nodes := [64]ConHashNode_t{}
	//node :=*C.struct_node_s{}
	var a *[0]byte = nil
	p := C.conhash_init(a)
	if p != nil {
		/* set nodes */
		C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&g_nodes[0])), C.int(1), C.CString("titanic"), C.uint(32))
		C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&g_nodes[1])), C.int(2), C.CString("terminator2018"), C.uint(24))
		C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&g_nodes[2])), C.int(3), C.CString("Xenomorph"), C.uint(25))
		C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&g_nodes[3])), C.int(4), C.CString("True Lies"), C.uint(10))
		C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&g_nodes[4])), C.int(5), C.CString("avantar"), C.uint(48))

		/* add nodes */
		C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&g_nodes[0])))
		C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&g_nodes[1])))
		C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&g_nodes[2])))
		C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&g_nodes[3])))
		C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&g_nodes[4])))

		fmt.Println("virtual nodes number ", C.conhash_get_vnodes_num(p))
		fmt.Println("the hashing results--------------------------------------:\n")

		var wg sync.WaitGroup
		for j := 0; j < 100; j++ {
			//go wr.sliDiskWrite[i].diskWrite(ctx, wr.chFileToDown, wr.chFileToIpfs)
			wg.Add(1)
			go func(j int) {
				for i := 0; i < 2000; i++ {
					//fmt.Printf(str, "James.km%03d", i)
					str := "James.km:" + strconv.Itoa(i) + "gonum:" + strconv.Itoa(j)
					node := C.conhash_lookup_go(p, C.CString(str), C.int(len(str)))

					fmt.Println(str, " is in node ", C.GoString(node))

				}
			}(j)
		}
		wg.Wait()
		/* try object */
		C.conhash_fini(p)
	}
}
