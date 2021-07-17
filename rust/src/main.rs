struct Node<T> {
    data: T,
    children: Vec<Node<T>>
}

impl<T: Default> Node<T> {
    fn new() -> Node<T> {
        Node {
            data: Default::default(),
            children: vec![]
        }
    } 
}

impl<T> Node<T> {
    fn new_with_initial(data: T) -> Node<T> {
        Node {
            data,
            children: vec![]
        }
    }

    fn append_child(&mut self, child: Node<T>) {
        self.children.push(child);
    }
    
    fn traverse(&mut self, modifier: &impl Fn(&T) -> T) {
        self.data = modifier(&self.data);
        self.children.iter_mut().for_each(|child| {
            child.traverse(modifier);
        });
    }
}

type IntT = u8;

const WIDTH: IntT = 100;

fn main() {
    let alloc_start = std::time::Instant::now();
    
    let mut root = Node::<IntT>::new();
    
    for x0 in 0..WIDTH {
        let mut l0 = Node::new_with_initial(x0);
        for x1 in 0..WIDTH {
            let mut l1 = Node::new_with_initial(x0 + x1);
            for x2 in 0..WIDTH {
                let l2 = Node::new_with_initial(x0 + x1 + x2);
                l1.append_child(l2);
            }
            l0.append_child(l1);
        }
        root.append_child(l0);
    }
    
    println!("allocated in: {}ms", alloc_start.elapsed().as_millis());

    for _ in 0..1000 {
        let iter_start = std::time::Instant::now();
        
        root.traverse(&|el| {
            el + 1
        });

        println!("iterated in: {}ms", iter_start.elapsed().as_millis());
    }

    root.traverse(&|el| {
        println!("{}", el);
        *el
    });
}