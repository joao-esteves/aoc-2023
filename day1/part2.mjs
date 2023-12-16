import { readFileSync } from 'node:fs'

function findFirstNumber (string) {
  const number = [...string].find(char => char >= '0' && char <= '9')
  if (number === undefined) throw 'no number found'
  return number
}

function findLastNumber (string) {
  return findFirstNumber(string.split('').reverse().join(''))
}

const digitMap = {
  o: { n: { e: 1 } },
  t: {
    w: { o: 2 },
    h: { r: { e: { e: 3 } } }
  },
  f: {
    o: { u: { r: 4 } },
    i: { v: { e: 5 } }
  },
  s: {
    i: { x: 6 },
    e: { v: { e: { n: 7 } } }
  },
  e: { i: { g: { h: { t: 8 } } } },
  n: { i: { n: { e: 9 } } }
}

// Outputs a string equal to the input but filtered so that only digits
// and textual representations of digits remain. For example:
// oneight => 18
// 2four5sia => 245
function transformNumbers (line) {
  const array = line.split('')
  const newArray = []
  // find starts of new digits (either in digit or textual form)
  for (let i = 0; i < array.length; i++) {
    const startChar = array[i]
    if (startChar >= '0' && startChar <= '9') newArray.push(startChar)
    if (!digitMap[startChar]) {
      continue
    }
    let nextChars = digitMap[startChar]

    // run textual form of digit until the end
    for (let j = i + 1; j < array.length; j++) {
      const currChar = array[j]
      if (typeof nextChars[currChar] === 'number') {
        newArray.push(nextChars[currChar])
        break
      }
      if (typeof nextChars[currChar] === 'undefined') break
      if (typeof nextChars[currChar] === 'object') {
        nextChars = nextChars[currChar]
      }
    }
  }
  return newArray.join('')
}

const lines = readFileSync('input.txt', 'utf-8').split(/\r?\n/)
const fullNumbersLines = lines.map(line => transformNumbers(line))
const sum = fullNumbersLines.reduce((accumulator, line) => {
  const number = Number('' + findFirstNumber(line) + findLastNumber(line))
  return accumulator + number
}, 0)
console.log(sum)

if (sum !== 53268) throw 'Wrong solution'
