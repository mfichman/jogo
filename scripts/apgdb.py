#/usr/bin/python


import re

string_type = gdb.lookup_type("String")
type_type = gdb.lookup_type("Type")


class StringPrinter():
    def __init__(self, value):
        self.value = value
    def to_string(self):
        tag = self.value.tag
        return self.value["string_"]["_M_dataplus"["_M_p"].string()
    def display_hint(self):
        return "string"

def PointerPrinter(value):
    pass

def pretty_print(value):
    tag = value.type.tag
    if tag == "String":
        return StringPrinter(value)
    return None
    
    


gdb.pretty_printers.append(pretty_print)
