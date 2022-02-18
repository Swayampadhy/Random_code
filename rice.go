package main

import (
	"fmt"
)

func main() {
	i := 0
	for {
		var x, y int
		go func(v *int) {
			*v = 60
		}(&x)
		go func(v *int) {
			*v = 3
		}(&y)
		go func(v1 int, v2 int) {
			fmt.Println(v1 / v2)
		}(x, y)

		i += 1

		fmt.Printf("%d\n", i)
	}
}