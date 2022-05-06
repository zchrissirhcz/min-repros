def print_int8x16_t(valobj, internal_dict):
    print(">>> print_int8x16_t")
    res = 'print_int8x16_t'
    return res

def print_uint8x16_t(valobj, internal_dict):
    print(">>> print_uint8x16_t")
    res = 'print_uint8x16_t'
    return res

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('type summary add -C no -P int8x16_t -F {:s}.print_int8x16_t'.format(__name__))
    debugger.HandleCommand('type summary add -C no -P uint8x16_t -F {:s}.print_uint8x16_t'.format(__name__))