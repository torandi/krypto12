#####
# SETTINGS
#####

alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"

newline_at_key_end=false

space_char = "_"
newline_char = "#"

####
# END SETTINGS
####

f = File.open(ARGV[0])
key = ARGV[1]

key_index = 0

data = f.read

data.each_char do |c|
  alphabet_index = alphabet.index c
  if alphabet_index
    g = alphabet.index key[key_index]
    key_index = (key_index+1)%key.length
    d = (alphabet_index-g)%alphabet.length
    letter = alphabet[d]
    letter = " " if letter == space_char
    letter = "\n" if !newline_at_key_end && letter == newline_char
    print letter
    if key_index == 0 && newline_at_key_end
      puts ""
    end
  end
end
