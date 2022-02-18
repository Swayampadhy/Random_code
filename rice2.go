package main

import (
  "fmt"
)
func add_one(pt *int) {
  (*pt)++
  fmt.Println(*pt)
}

func sub_one(pt *int) {
  (*pt)--
  fmt.Println(*pt)
}
func main() {
  i := 0
  go add_one(&i)
  go sub_one(&i)

  i++
  fmt.Println()

}