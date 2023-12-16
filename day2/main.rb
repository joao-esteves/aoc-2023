# frozen_string_literal: true

# Determine which games would have been possible if the bag had been loaded
# with only 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum
# of the IDs of those games?

# Line format:
# Game <ID>: [<RED_n> red, <BLUE_n> blue, <GREEN_n> green][multiple times separated by ;]

MAX_RED = 12
MAX_GREEN = 13
MAX_BLUE = 14

sum = 0

File.readlines('input.txt', chomp: true).each do |line|
  game_str, combos = line.split ':'
  game_id = game_str.split(' ')[1].to_i
  valid = nil
  combos.strip.split(';').each do |combo|
    combo.strip.split(',').each do |color_pair|
      num, color = color_pair.split ' '
      num = num.to_i
      valid = (color == 'red' && num <= MAX_RED) ||
              (color == 'green' && num <= MAX_GREEN) ||
              (color == 'blue' && num <= MAX_BLUE)
      break unless valid
    end
    break unless valid
  end
  sum += game_id if valid
end

puts sum

raise 'Wrong solution' if sum != 1931
