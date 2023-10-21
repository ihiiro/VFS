# VFS (Virtual File System)

## VSD-DRIVER

### Purpose
The VSD-DRIVER is what interfaces directly with the VSD (Virtual Storage Drive) which is an empty (but allocated) file, this file is viewed by VFS as a storage drive and thus treated as such.

### API
```void    read_block_to_buffer(int block_index);```

Reads a full block from VSD into the central read buffer, block_index is 0 indexed.

```void    write_to_block(int block_index, int offset, char *buf, int size);```

Writes the contents of `buf` into the VSD at `block_index` at a specific `offset` within the block (both zero indexed).

```char    *return_buffer();```

Returns the central read buffer.

```off_t   return_vsd_size();```

Returns the total VSD size in bytes.

```struct driver_status return_driver_status();```

Returns DRIVER_STATUS struct, which contains the latest DRIVER_STATUS entry.

```sll_headnode_t   *return_driver_status_log();```

Returns a pointer to the head of the DRIVER_STATUS_LOG singly linked list, the list contains the full log of all DRIVER_STATUS entries for the entire runtime duration.

```void initialize_driver_status();```

Allocates memory for DRIVER_STATUS.status and creates a new DRIVER_STATUS_LOG singly linked list.

```void free_driver_status();```

Frees both DRIVER_STATUS.status and the entire DRIVER_STATUS_LOG from memory.

(note: DRIVER_STATUS_LOG uses a linked list I wrote in a separate project, for the API check https://github.com/ihiiro/singly_linked_list)