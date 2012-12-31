
obj = {}
x = 0
obj.call = function(self) 
    x = x + 1 
end
     

i = 0
while i < 100000000 do
    obj:call()
    i = i + 1
end

print(x)
