
$x = 0
class Obj 
    def call() 
        $x = $x + 1
    end

end


obj = Obj.new()
i = 0
while i < 100000000 
    obj.call()
    i = i + 1
end

puts "#{$x}"

