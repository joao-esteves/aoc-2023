# frozen_string_literal: true

# For each game, find the minimum set of cubes that must have been present.
# What is the sum of the power of these sets?

# Line format:
# Game <ID>: [<RED_n> red, <BLUE_n> blue, <GREEN_n> green][multiple times separated by ;]

MAX_RED = 12
MAX_GREEN = 13
MAX_BLUE = 14

sum = 0

File.readlines('input.txt', chomp: true).each do |line|
  _game_str, combos = line.split ':'
  min_red = min_green = min_blue = 0
  combos.strip.split(';').each do |combo|
    combo.strip.split(',').each do |color_pair|
      num, color = color_pair.split ' '
      num = num.to_i
      min_red = num if color == 'red' && num > min_red
      min_green = num if color == 'green' && num > min_green
      min_blue = num if color == 'blue' && num > min_blue
    end
  end
  sum += min_red * min_green * min_blue
end

puts sum

raise 'Wrong solution' if sum != 83105
