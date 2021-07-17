import java.lang.ref.WeakReference
import kotlin.system.measureTimeMillis

class Node<T>(var data: T) {
    val children = mutableListOf<Node<T>>();

    fun append_child(child: Node<T>) {
        children.add(child);
    }

    fun traverse(modifier: (T) -> T) {
        data = modifier(data);

        children.forEach {
            it.traverse(modifier)
        }
    }
}

const val WID = 100u;

fun main() {
    val root: Node<UByte>;

    val allocTime = measureTimeMillis {
        root = Node(2u);

        for (x0 in 0u until WID) {
            val l0 = Node(x0.toUByte());
            for (x1 in 0u until WID) {
                val l1 = Node((x0 + x1).toUByte());
                for (x2 in 0u until WID) {
                    val l2 = Node((x0 + x1 + x2).toUByte());
                    l1.append_child(l2);
                }
                l0.append_child(l1);
            }
            root.append_child(l0);
        }
    }

    println("allocated in: ${allocTime}ms");

    for (i in 0 until 1000) {
        val iterTime = measureTimeMillis {
            root.traverse {
                (it + 1u).toUByte()
            }
        };
        println("iterated in: ${iterTime}ms");
    }

    root.traverse {
        print(it);
        it
    }
}