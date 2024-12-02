fn main() {
    println!("벡터 1 : ");
    let vec1 = read_vector();
    println!("벡터 2 : ");
    let vec2 = read_vector();
    
    let vec3 = add_vector(&vec1, &vec2);
    
    for value in &vec3 {
        print!("{} ", value);
    }
}

fn read_vector() -> Vec<i32>{
    let mut str = String::new();
    std::io::stdin().read_line(&mut str).expect("failed to read from stdin");
    str
        .split_whitespace()
        .map(|str| str.parse::<i32>().unwrap())
        .collect::<Vec<_>>()
}

fn add_vector(vec1: &Vec<i32>, vec2: &Vec<i32>) -> Vec<i32>{
    let mut v = Vec::new();
    let size = vec1.len();
    for i in 0..size {
        v.push(vec1[i] + vec2[i]);
    }
    v
}