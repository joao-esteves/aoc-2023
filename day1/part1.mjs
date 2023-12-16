import { readFileSync } from 'node:fs'

function findFirstDigit (string) {
  const number = [...string].find(char => char >= '0' && char <= '9')
  if (number === undefined) throw 'no number found'
  return number
}

function findLastDigit (string) {
  return findFirstDigit(string.split('').reverse().join(''))
}

const lines = readFileSync('input.txt', 'utf-8').split(/\r?\n/)
const sum = lines.reduce((accumulator, line) => {
  const number = Number('' + findFirstDigit(line) + findLastDigit(line))
  return accumulator + number
}, 0)
console.log(sum)

if (sum !== 53080) throw 'Wrong solution'
