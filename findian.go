package main

import "fmt"

func main() {
	var string_input string
	fmt.Println("Enter a string")
	fmt.Scan(&string_input)
	for arune := range string_input {
		switch {
		case arune == 'i':
			fmt.Println("Found!")
		case arune == 'a':
			fmt.Println("Found!")
		case arune == 'n':
			fmt.Println("Found!")
		default:
			fmt.Println("Not Found!")
		}
	}
}
