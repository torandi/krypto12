f = File.open(ARGV[0])

data = f.read

pos = 0
hex_str = "00"

#key_range = (0..0x7f)
key_range = [0x6e]

replace_map = {
  "_"=>"o",
  "O"=>" ",
  "?" => "O",
  "#"=>"\n"
}

print_newline = false
performe_replace = true

key_range.each do |key|
  puts "\n================ BEGIN KEY #{key} =================\n"
  data.each_char do |c|
    if c != "\n" 
      hex_str[pos]=c;
      pos+=1
      if(pos == 2)
        num = (hex_str.to_i(16)+key)%0x7f
        char = num.chr
        if(performe_replace && replace_map[char])
          char = replace_map[char]
        end
        print char
        pos = 0;
      end
    elsif print_newline
      puts "\n"
    end
  end
  puts "\n================ END KEY #{key} =================\n"
end
