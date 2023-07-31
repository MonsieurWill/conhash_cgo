package conhash

// #cgo CFLAGS: -I${SRCDIR}/
// #cgo LDFLAGS: -L${SRCDIR}/../libs -lconhash
// #include "../cconhash/configure.h"
// #include "../cconhash/conhash.h"
import "C"
import (
	"sync"
	"unsafe"
)

type ConHashNode struct {
	Iden     [64]int8
	Replicas uint32
	Index    uint32
	Flag     uint32
}
type RBTreeNode struct {
	Key    int64
	Parent *RBTreeNode
	Right  *RBTreeNode
	Left   *RBTreeNode
	Color  int32
	Data   *byte
}
type RBTree struct {
	Root      *RBTreeNode
	Null      RBTreeNode
	Size      uint32
	Pad_cgo_0 [4]byte
}
type ConHash struct {
	pConHash *C.struct_conhash_s
}

var instance *ConHash
var once sync.Once
var gNodes = [64]ConHashNode{}
var mapDirCap map[string]int64

func conHashInit() *C.struct_conhash_s {
	var a *[0]byte = nil
	p := C.conhash_init(a)
	if p != nil {
		i := 0
		for strDirPath, i64cap := range mapDirCap {
			i64VirNodes := i64cap
			C.conhash_set_node((*C.struct_node_s)(unsafe.Pointer(&gNodes[i])), C.int(i), C.CString(strDirPath), C.uint(i64VirNodes))
			C.conhash_add_node(p, (*C.struct_node_s)(unsafe.Pointer(&gNodes[i])))
			i++
		}

	}
	return p
}

func newConhashHandle() *ConHash {
	pConHash := conHashInit()
	return &ConHash{
		pConHash: pConHash,
	}
}

func InitGetConHash() *ConHash {
	once.Do(func() {
		instance = newConhashHandle()
	})
	return instance
}

func InitConHash(f func() map[string]int64) bool {
	mapDirCap = f()
	InitGetConHash()
	return true
}

func (c *ConHash) ConHashLookUp(key string) string {
	//var node *C.char
	node := C.conhash_lookup_go(c.pConHash, C.CString(key), C.int(len(key)))
	return C.GoString(node)
}
