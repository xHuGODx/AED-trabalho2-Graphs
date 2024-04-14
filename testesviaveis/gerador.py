
def write_combinations_to_file(n, filename='8vertices.txt'):
    with open(filename, 'w') as file:
        file.write(f'1\n0\n{n}\n')

        total_lines = sum(n - i for i in range(0, n))
        file.write(f'{total_lines}\n')

        for i in range(0, n):
            for j in range(i + 1, n):
                file.write(f'{i} {j}\n')

def main():
    n = 8
    write_combinations_to_file(n)


def generate_files():
    vertices_list = [2048, 4096, 8192, 16384, 32768]
    for vertices in vertices_list:
        output_filename = f'{vertices}vertices.txt'
        write_combinations_to_file(vertices, output_filename)
        print(f'The combinations for {vertices} vertices are written to "{output_filename}".')


if __name__ == "__main__":
    generate_files()
