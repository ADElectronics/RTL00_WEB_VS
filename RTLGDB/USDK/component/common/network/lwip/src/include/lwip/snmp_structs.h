/**
 * @file
 * Generic MIB tree structures.
 *
 * @todo namespace prefixes
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Christiaan Simons <christiaan.simons@axon.tv>
 */

#ifndef __LWIP_SNMP_STRUCTS_H__
#define __LWIP_SNMP_STRUCTS_H__

#include "lwip/opt.h"

#if LWIP_SNMP /* don't build if not configured for use in lwipopts.h */

#include "lwip/snmp.h"

#if SNMP_PRIVATE_MIB
/* When using a private MIB, you have to create a file 'private_mib.h' that contains
 * a 'struct mib_array_node mib_private' which contains your MIB. */
#include "private_mib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* MIB object instance */
#define MIB_OBJECT_NONE 0 
#define MIB_OBJECT_SCALAR 1
#define MIB_OBJECT_TAB 2

/* MIB access types */
#define MIB_ACCESS_READ   1
#define MIB_ACCESS_WRITE  2

/* MIB object access */
#define MIB_OBJECT_READ_ONLY      MIB_ACCESS_READ
#define MIB_OBJECT_READ_WRITE     (MIB_ACCESS_READ | MIB_ACCESS_WRITE)
#define MIB_OBJECT_WRITE_ONLY     MIB_ACCESS_WRITE
#define MIB_OBJECT_NOT_ACCESSIBLE 0

/** object definition returned by (get_object_def)() */
struct obj_def
{
  /* MIB_OBJECT_NONE (0), MIB_OBJECT_SCALAR (1), MIB_OBJECT_TAB (2) */
  uint8_t instance;
  /* 0 read-only, 1 read-write, 2 write-only, 3 not-accessible */
  uint8_t access;
  /* ASN type for this object */
  uint8_t asn_type;
  /* value length (host length) */
  uint16_t v_len;
  /* length of instance part of supplied object identifier */
  uint8_t  id_inst_len;
  /* instance part of supplied object identifier */
  int32_t *id_inst_ptr;
};

struct snmp_name_ptr
{
  uint8_t ident_len;
  int32_t *ident;
};

/** MIB const scalar (.0) node */
#define MIB_NODE_SC 0x01
/** MIB const array node */
#define MIB_NODE_AR 0x02
/** MIB array node (mem_malloced from RAM) */
#define MIB_NODE_RA 0x03
/** MIB list root node (mem_malloced from RAM) */
#define MIB_NODE_LR 0x04
/** MIB node for external objects */
#define MIB_NODE_EX 0x05

/** node "base class" layout, the mandatory fields for a node  */
struct mib_node
{
  /** returns struct obj_def for the given object identifier */
  void (*get_object_def)(uint8_t ident_len, int32_t *ident, struct obj_def *od);
  /** returns object value for the given object identifier,
     @note the caller must allocate at least len bytes for the value */
  void (*get_value)(struct obj_def *od, uint16_t len, void *value);
  /** tests length and/or range BEFORE setting */
  uint8_t (*set_test)(struct obj_def *od, uint16_t len, void *value);
  /** sets object value, only to be called when set_test()  */
  void (*set_value)(struct obj_def *od, uint16_t len, void *value);  
  /** One out of MIB_NODE_AR, MIB_NODE_LR or MIB_NODE_EX */
  uint8_t node_type;
  /* array or max list length */
  uint16_t maxlength;
};

/** derived node for scalars .0 index */
typedef struct mib_node mib_scalar_node;

/** derived node, points to a fixed size const array
    of sub-identifiers plus a 'child' pointer */
struct mib_array_node
{
  /* inherited "base class" members */
  void (*get_object_def)(uint8_t ident_len, int32_t *ident, struct obj_def *od);
  void (*get_value)(struct obj_def *od, uint16_t len, void *value);
  uint8_t (*set_test)(struct obj_def *od, uint16_t len, void *value);
  void (*set_value)(struct obj_def *od, uint16_t len, void *value);

  uint8_t node_type;
  uint16_t maxlength;

  /* additional struct members */
  const int32_t *objid;
  struct mib_node* const *nptr;
};

/** derived node, points to a fixed size mem_malloced array
    of sub-identifiers plus a 'child' pointer */
struct mib_ram_array_node
{
  /* inherited "base class" members */
  void (*get_object_def)(uint8_t ident_len, int32_t *ident, struct obj_def *od);
  void (*get_value)(struct obj_def *od, uint16_t len, void *value);
  uint8_t (*set_test)(struct obj_def *od, uint16_t len, void *value);
  void (*set_value)(struct obj_def *od, uint16_t len, void *value);

  uint8_t node_type;
  uint16_t maxlength;

  /* aditional struct members */
  int32_t *objid;
  struct mib_node **nptr;
};

struct mib_list_node
{
  struct mib_list_node *prev;  
  struct mib_list_node *next;
  int32_t objid;
  struct mib_node *nptr;
};

/** derived node, points to a doubly linked list
    of sub-identifiers plus a 'child' pointer */
struct mib_list_rootnode
{
  /* inherited "base class" members */
  void (*get_object_def)(uint8_t ident_len, int32_t *ident, struct obj_def *od);
  void (*get_value)(struct obj_def *od, uint16_t len, void *value);
  uint8_t (*set_test)(struct obj_def *od, uint16_t len, void *value);
  void (*set_value)(struct obj_def *od, uint16_t len, void *value);

  uint8_t node_type;
  uint16_t maxlength;

  /* additional struct members */
  struct mib_list_node *head;
  struct mib_list_node *tail;
  /* counts list nodes in list  */
  uint16_t count;
};

/** derived node, has access functions for mib object in external memory or device
    using 'tree_level' and 'idx', with a range 0 .. (level_length() - 1) */
struct mib_external_node
{
  /* inherited "base class" members */
  void (*get_object_def)(uint8_t ident_len, int32_t *ident, struct obj_def *od);
  void (*get_value)(struct obj_def *od, uint16_t len, void *value);
  uint8_t (*set_test)(struct obj_def *od, uint16_t len, void *value);
  void (*set_value)(struct obj_def *od, uint16_t len, void *value);

  uint8_t node_type;
  uint16_t maxlength;

  /* additional struct members */
  /** points to an external (in memory) record of some sort of addressing
      information, passed to and interpreted by the funtions below */
  void* addr_inf;
  /** tree levels under this node */
  uint8_t tree_levels;
  /** number of objects at this level */
  uint16_t (*level_length)(void* addr_inf, uint8_t level);
  /** compares object sub identifier with external id
      return zero when equal, nonzero when unequal */
  int32_t (*ident_cmp)(void* addr_inf, uint8_t level, uint16_t idx, int32_t sub_id);
  void (*get_objid)(void* addr_inf, uint8_t level, uint16_t idx, int32_t *sub_id);

  /** async Questions */
  void (*get_object_def_q)(void* addr_inf, uint8_t rid, uint8_t ident_len, int32_t *ident);
  void (*get_value_q)(uint8_t rid, struct obj_def *od);
  void (*set_test_q)(uint8_t rid, struct obj_def *od);
  void (*set_value_q)(uint8_t rid, struct obj_def *od, uint16_t len, void *value);
  /** async Answers */
  void (*get_object_def_a)(uint8_t rid, uint8_t ident_len, int32_t *ident, struct obj_def *od);
  void (*get_value_a)(uint8_t rid, struct obj_def *od, uint16_t len, void *value);
  uint8_t (*set_test_a)(uint8_t rid, struct obj_def *od, uint16_t len, void *value);
  void (*set_value_a)(uint8_t rid, struct obj_def *od, uint16_t len, void *value);
  /** async Panic Close (agent returns error reply, 
      e.g. used for external transaction cleanup) */
  void (*get_object_def_pc)(uint8_t rid, uint8_t ident_len, int32_t *ident);
  void (*get_value_pc)(uint8_t rid, struct obj_def *od);
  void (*set_test_pc)(uint8_t rid, struct obj_def *od);
  void (*set_value_pc)(uint8_t rid, struct obj_def *od);
};

/** export MIB tree from mib2.c */
extern const struct mib_array_node internet;

/** dummy function pointers for non-leaf MIB nodes from mib2.c */
void noleafs_get_object_def(uint8_t ident_len, int32_t *ident, struct obj_def *od);
void noleafs_get_value(struct obj_def *od, uint16_t len, void *value);
uint8_t noleafs_set_test(struct obj_def *od, uint16_t len, void *value);
void noleafs_set_value(struct obj_def *od, uint16_t len, void *value);

void snmp_oidtoip(int32_t *ident, ip_addr_t *ip);
void snmp_iptooid(ip_addr_t *ip, int32_t *ident);
void snmp_ifindextonetif(int32_t ifindex, struct netif **netif);
void snmp_netiftoifindex(struct netif *netif, int32_t *ifidx);

struct mib_list_node* snmp_mib_ln_alloc(int32_t id);
void snmp_mib_ln_free(struct mib_list_node *ln);
struct mib_list_rootnode* snmp_mib_lrn_alloc(void);
void snmp_mib_lrn_free(struct mib_list_rootnode *lrn);

int8_t snmp_mib_node_insert(struct mib_list_rootnode *rn, int32_t objid, struct mib_list_node **insn);
int8_t snmp_mib_node_find(struct mib_list_rootnode *rn, int32_t objid, struct mib_list_node **fn);
struct mib_list_rootnode *snmp_mib_node_delete(struct mib_list_rootnode *rn, struct mib_list_node *n);

struct mib_node* snmp_search_tree(struct mib_node *node, uint8_t ident_len, int32_t *ident, struct snmp_name_ptr *np);
struct mib_node* snmp_expand_tree(struct mib_node *node, uint8_t ident_len, int32_t *ident, struct snmp_obj_id *oidret);
uint8_t snmp_iso_prefix_tst(uint8_t ident_len, int32_t *ident);
uint8_t snmp_iso_prefix_expand(uint8_t ident_len, int32_t *ident, struct snmp_obj_id *oidret);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_SNMP */

#endif /* __LWIP_SNMP_STRUCTS_H__ */
