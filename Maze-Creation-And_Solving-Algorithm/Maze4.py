import pygame
import sys
import random
import time
from pygame.locals import *

# Constants
screen_size = width, height = 870, 860
black = (0, 0, 0)
grey = (160, 160, 160)
blue = (255, 192, 203)
FPS = 30


def main():
    consoleUI()


def consoleUI():
    print("Hello, this is a maze creation and solving program")
    createq = input("Would you like to create a maze (Y/N)---").upper()
    while createq not in ["Y", "N"]:
        print("Invalid input")
        createq = input("Please enter Y or N ---").upper()
    if createq == "N":
        sys.exit()
    print("What type of maze would you like to create?")
    maze_type = input("Type P for Prim's and R for RBT ---").upper()
    while maze_type not in ["P", "R"]:
        print("Invalid input")
        maze_type = input("Please enter P or R ---").upper()
    global size
    global cell_width
    use_diff = input("Would you like to select a difficulty level (Y/N)---").upper()
    while use_diff not in ["Y", "N"]:
        print("Invalid input")
        use_diff = input("Please enter Y or N ---").upper()
    if use_diff == "Y":
        diff = input("(E)asy, (M)edium, (H)ard ---").upper()
        while diff not in ["E", "M", "H"]:
            print("Invalid input")
            diff = input("Please enter E, M or H ---").upper()
        if diff == "E":
            maze_size = 10
        elif diff == "M":
            maze_size = 30
        else:
            maze_size = 55
    else:
        maze_size = int(input("Enter maze size ---"))
        while maze_size < 2 or maze_size > 100:
            print("Invalid input")
            maze_size = int(input("Please enter a whole number between 2 and 100 ---"))
    size = maze_size
    cell_width = (width - 5) // size
    if cell_width > 15:
        cell_width = 15
    m = Maze()
    if maze_type == "P":
        m.create_maze_prim()
        cprint = input("Would you like to print maze to console? (Y/N) ---").upper()
        while cprint not in ["Y", "N"]:
            print("Invalid input")
            cprint = input("Please enter Y or N ---").upper()
        if cprint == "Y":
            m.print_maze("prim")
        animate = input("Would you like to animate maze? (Y/N) ---").upper()
        while animate not in ["Y", "N"]:
            print("Invalid input")
            animate = input("Please enter Y or N ---").upper()
        if animate == "Y":
            time.sleep(3)
            Pygame(m.get_grid_prim(), m.get_animation_prim()).animate_grid()
        else:
            Pygame(m.get_grid_prim(), m.get_animation_prim()).draw_maze(blue)
            pygame.event.get()
            pygame.display.update()
        pygame.image.save(window, "Maze.jpg")
        solve = input("Would you like to solve maze? (Y/N) ---").upper()
        while solve not in ["Y", "N"]:
            print("Invalid input")
            solve = input("Please enter Y or N ---").upper()
        if solve == "Y":
            x = Pathfinding(m.get_grid_prim())
            x.a_star()
            Pygame(m.get_grid_prim(), m.get_animation_prim()).draw_maze(blue)
            pygame.display.update()
            animate = input("Would you like to animate solution? (Y/N) ---").upper()
            while animate not in ["Y", "N"]:
                print("Invalid input")
                animate = input("Please enter Y or N ---").upper()
            if animate == "Y":
                time.sleep(3)
                Pygame(m.get_grid_prim(), x.get_visited()).animate_solution()
            Pygame(m.get_grid_prim(), x.get_path()).draw_solution()
            pygame.display.update()
    if maze_type == "R":
        m.create_maze_rbt()
        cprint = input("Would you like to print maze to console? (Y/N) ---").upper()
        while cprint not in ["Y", "N"]:
            print("Invalid input")
            cprint = input("Please enter Y or N ---").upper()
        if cprint == "Y":
            m.print_maze("rbt")
        animate = input("Would you like to animate maze? (Y/N) ---").upper()
        while animate not in ["Y", "N"]:
            print("Invalid input")
            animate = input("Please enter Y or N ---").upper()
        time.sleep(3)
        if animate == "Y":
            Pygame(m.get_grid_rbt(), m.get_animation_rtb()).animate_grid()
        else:
            Pygame(m.get_grid_rbt(), m.get_animation_rtb()).draw_maze(blue)
            pygame.event.get()
            pygame.display.update()
        pygame.image.save(window, "Maze.jpg")
        solve = input("Would you like to solve maze? (Y/N) ---").upper()
        while solve not in ["Y", "N"]:
            print("Invalid input")
            solve = input("Please enter Y or N ---").upper()
        if solve == "Y":
            x = Pathfinding(m.get_grid_rbt())
            x.a_star()
            Pygame(m.get_grid_rbt(), m.get_animation_rtb()).draw_maze(blue)
            pygame.display.update()
            animate = input("Would you like to animate solution? (Y/N) ---").upper()
            while animate not in ["Y", "N"]:
                print("Invalid input")
                animate = input("Please enter Y or N ---").upper()
            if animate == "Y":
                time.sleep(3)
                Pygame(m.get_grid_rbt(), x.get_visited()).animate_solution()
            Pygame(m.get_grid_rbt(), x.get_path()).draw_solution()
            pygame.display.update()
    save = input("Would you like save maze as image? (Y/N) ---").upper()
    while save not in ["Y", "N"]:
        print("Invalid input")
        save = input("Please enter Y or N ---").upper()
    if save == "Y":
        pygame.image.save(window, "Maze.jpg")
    export = input("Would you like to export maze? (Y/N) ---").upper()
    while export not in ["Y", "N"]:
        print("Invalid input")
        export = input("Please enter Y or N ---").upper()
    if export == "Y":
        if maze_type == "R":
            exportMaze(m.get_grid_rbt())
        else:
            exportMaze(m.get_grid_prim())
    s_maze = input("Would you like to create another maze? (Y/N) ---").upper()
    while s_maze not in ["Y", "N"]:
        print("Invalid input")
        s_maze = input("Please enter Y or N ---").upper()
    if s_maze == "Y":
        window.fill(black)
        consoleUI()
    sys.exit()

def exportMaze(maze):
    f = open("maze.txt", "w")
    for row in range(len(maze)):
        for column in range(len(maze)):
            cell = maze[row][column]
            line = f"({cell.x},{cell.y}),{cell.get_walls()}\n"
            f.write(line)
    f.close()




class Cell:  # Stores certain attributes for each cell in the maze
    def __init__(self, x, y):
        # Stores the coordinates of the cell
        self.x = x
        self.y = y
        # Initially sets all the walls of the cell to True
        self._up = True
        self._down = True
        self._left = True
        self._right = True

    def get_walls(self):  # Returns the list of walls a cell has
        return [self._up, self._down, self._left, self._right]

    def set_wall(self, wall):  # Removes a certain wall from a cell
        if wall == "up":
            self._up = False
        elif wall == "down":
            self._down = False
        elif wall == "left":
            self._left = False
        elif wall == "right":
            self._right = False
        if wall == 0:
            self._up = False
        elif wall == 1:
            self._down = False
        elif wall == 2:
            self._left = False
        elif wall == 3:
            self._right = False


class Maze:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.size = size
        self.grid = []
        self.animation_rtb = []
        self.animation_prim = []
        self.build_grid()
        self.grid_rbt = self.grid
        self.grid_prim = self.grid

    def print_maze(self, algorithm=None):
        vstring = "+"
        hstring = ""
        string = ""
        if algorithm == "rbt":
            grid = self.grid_rbt
        elif algorithm == "prim":
            grid = self.grid_prim
        else:
            grid = self.grid
        for row in range(self.size):
            for column in range(self.size):
                walls = grid[row][column].get_walls()
                if walls[2]:
                    hstring += "|  "
                else:
                    hstring += "   "
                if walls[0]:
                    vstring += "--+"
                else:
                    vstring += "  +"
            string += vstring + "\n" + hstring + "|\n"
            hstring = ""
            vstring = "+"
        print(string + "+--" * self.size + "+")

    def build_grid(self):
        for i in range(self.size):
            self.grid.append([])
            for j in range(self.size):
                cell = Cell(i, j)
                self.grid[i].append(cell)

    def create_maze_rbt(self):
        stack = []
        visited = []
        coords = [(x, y) for x in range(self.size) for y in range(self.size)]
        x = 0
        y = 0
        stack.append((x, y))
        visited.append((x, y))
        while len(stack) > 0:
            path = []
            if (x + 1, y) not in visited and (x + 1, y) in coords:
                path.append("right")

            if (x - 1, y) not in visited and (x - 1, y) in coords:
                path.append("left")

            if (x, y + 1) not in visited and (x, y + 1) in coords:
                path.append("down")

            if (x, y - 1) not in visited and (x, y - 1) in coords:
                path.append("up")

            if len(path) > 0:
                current_path = random.choice(path)
                self.animation_rtb.append((x, y, current_path))
                if current_path == "right":
                    self.grid_rbt[y][x].set_wall(current_path)
                    x += 1
                    self.grid_rbt[y][x].set_wall("left")
                    visited.append((x, y))
                    stack.append((x, y))

                elif current_path == "left":
                    self.grid_rbt[y][x].set_wall(current_path)
                    x -= 1
                    self.grid_rbt[y][x].set_wall("right")
                    visited.append((x, y))
                    stack.append((x, y))

                elif current_path == "up":
                    self.grid_rbt[y][x].set_wall(current_path)
                    y -= 1
                    self.grid_rbt[y][x].set_wall("down")
                    visited.append((x, y))
                    stack.append((x, y))

                elif current_path == "down":
                    self.grid_rbt[y][x].set_wall(current_path)
                    y += 1
                    self.grid_rbt[y][x].set_wall("up")
                    visited.append((x, y))
                    stack.append((x, y))

            else:
                x, y = stack.pop()

    def get_grid_rbt(self):
        return self.grid_rbt

    def create_maze_prim(self):
        coords = [(x, y) for x in range(self.size) for y in range(self.size)]
        walls = []
        visited = []
        cell = self.grid[0][0]
        x, y = cell.x, cell.y
        walls = walls + self.add_walls(x, y)
        visited.append((x, y))
        while len(walls) > 0:
            path = random.choice(walls)
            walls.pop(walls.index(path))
            visitNum = 0
            x, y = path[0], path[1]
            if path[-1] == "up":
                if (x, y - 1) not in visited and (x, y - 1) in coords:
                    self.animation_prim.append(path)
                    self.grid_prim[y][x].set_wall(path[-1])
                    y -= 1
                    self.grid_prim[y][x].set_wall("down")
                    visited.append((x, y))
                    walls = walls + self.add_walls(x, y)
                    visitNum += 1
            elif path[-1] == "down":
                if (x, y + 1) not in visited and (x, y + 1) in coords:
                    self.animation_prim.append(path)
                    self.grid_prim[y][x].set_wall(path[-1])
                    y += 1
                    self.grid_prim[y][x].set_wall("up")
                    visited.append((x, y))
                    walls = walls + self.add_walls(x, y)
                    visitNum += 1
            elif path[-1] == "left":
                if (x - 1, y) not in visited and (x - 1, y) in coords:
                    self.animation_prim.append(path)
                    self.grid_prim[y][x].set_wall(path[-1])
                    x -= 1
                    self.grid_prim[y][x].set_wall("right")
                    visited.append((x, y))
                    walls = walls + self.add_walls(x, y)
                    visitNum += 1
            elif path[-1] == "right":
                if (x + 1, y) not in visited and (x + 1, y) in coords:
                    self.animation_prim.append(path)
                    self.grid_prim[y][x].set_wall(path[-1])
                    x += 1
                    self.grid_prim[y][x].set_wall("left")
                    visited.append((x, y))
                    walls = walls + self.add_walls(x, y)
                    visitNum += 1

    def add_walls(self, x, y):
        walls = []
        cell_walls = self.grid_prim[y][x].get_walls()
        if cell_walls[0] and y != 0:
            walls.append((x, y, "up"))
        if cell_walls[1] and y != self.size - 1:
            walls.append((x, y, "down"))
        if cell_walls[2] and x != 0:
            walls.append((x, y, "left"))
        if cell_walls[3] and x != self.size - 1:
            walls.append((x, y, "right"))
        return walls

    def get_grid_prim(self):
        return self.grid_prim

    def get_animation_rtb(self):
        return self.animation_rtb

    def get_animation_prim(self):
        return self.animation_prim


class Pygame:
    def __init__(self, maze, animation):
        self.size = size
        self.white = (255, 255, 255)
        self.black = black
        self.width = cell_width
        self.animation = animation
        self.grid = maze
        self.green = (0, 255, 0,)
        self.blue = (255, 192, 203)
        self.red = (255, 0, 0)
        self.lblue = (45, 213, 200)
        self.dblue = (1, 95, 96)

    def animate_grid(self):
        color = self.blue
        self.draw_grid(color)
        for cell in self.animation:
            x, y = cell[0] + 1, cell[1] + 1
            x, y = x * self.width, y * self.width
            # print(cell)
            time.sleep(.001)
            if cell[-1] == "up":
                pygame.draw.line(window, self.black, [x, y], [x + self.width, y])  # Draws top wall of cell
                pygame.event.get()
                pygame.display.update()

            if cell[-1] == "down":
                pygame.draw.line(window, self.black, [x + self.width, y + self.width],
                                 [x, y + self.width])  # Draws bottom wall of cell
                pygame.event.get()
                pygame.display.update()

            if cell[-1] == "left":
                pygame.draw.line(window, self.black, [x, y + self.width], [x, y])  # Draws left wall of cell
                pygame.event.get()
                pygame.display.update()

            if cell[-1] == "right":
                pygame.draw.line(window, self.black, [x + self.width, y],
                                 [x + self.width, y + self.width])  # Draws right wall of cell
                pygame.event.get()
                pygame.display.update()
        self.draw_maze(color)

    def draw_grid(self, color):
        for row in range(self.size):
            for column in range(self.size):
                x, y = row + 1, column + 1
                x, y = x * self.width, y * self.width
                pygame.draw.line(window, color, [x, y], [x + self.width, y])  # Draws top wall of cell
                pygame.draw.line(window, color, [x + self.width, y + self.width],
                                 [x, y + self.width])  # Draws bottom wall of cell
                pygame.draw.line(window, color, [x, y + self.width], [x, y])  # Draws left wall of cell
                pygame.draw.line(window, color, [x + self.width, y],
                                 [x + self.width, y + self.width])  # Draws right wall of cell
        pygame.draw.rect(window, self.green, (self.width + 2, self.width + 2, self.width - 4, self.width - 4), 0)
        pygame.draw.rect(window, self.red,
                         (self.width * self.size + 3, self.width * self.size + 2, self.width - 4, self.width - 4), 0)

    def draw_maze(self, color):
        for row in range(self.size):
            for column in range(self.size):
                cell = self.grid[row][column]
                walls = cell.get_walls()
                x, y = cell.x + 1, cell.y + 1
                y, x = x * self.width, y * self.width
                if walls[0]:
                    pygame.draw.line(window, color, [x, y], [x + self.width, y])  # Draws top wall of cell
                if walls[1]:
                    pygame.draw.line(window, color, [x + self.width, y + self.width],
                                     [x, y + self.width])  # Draws bottom wall of cell
                if walls[2]:
                    pygame.draw.line(window, color, [x, y + self.width], [x, y])  # Draws left wall of cell
                if walls[3]:
                    pygame.draw.line(window, color, [x + self.width, y],
                                     [x + self.width, y + self.width])  # Draws right wall of cell
        pygame.draw.rect(window, self.green, (self.width + 2, self.width + 2, self.width - 4, self.width - 4), 0)
        pygame.draw.rect(window, self.red,
                         (self.width * self.size + 3, self.width * self.size + 2, self.width - 4, self.width - 4), 0)

    def animate_solution(self):
        color = self.lblue
        for cell in self.animation:
            time.sleep(.007)
            x, y = cell[0] + 1, cell[1] + 1
            y, x = x * self.width, y * self.width
            width = self.width / 2
            pygame.draw.circle(window, color, (x + width, y + width), 2, 2)
            pygame.event.get()
            pygame.display.update()

    def draw_solution(self):
        color = self.red
        for cell in self.animation:
            x, y = cell[0] + 1, cell[1] + 1
            y, x = x * self.width, y * self.width
            width = self.width / 2
            pygame.draw.circle(window, color, (x + width, y + width), 2, 2)


class Pathfinding:
    def __init__(self, maze):
        self.size = size
        self.maze = maze
        self.path = []
        self.visited = 0

    def a_star(self):
        start = (0, 0)
        end = self.maze[self.size - 1][self.size - 1]
        self.path.append((0, 0))
        found = False
        visited = [(0, 0)]
        p_route = []
        w = 0
        while not found:
            adj = []
            adj = self.get_adjacent_cells(self.path[-1][0], self.path[-1][1])
            cost = []
            coords = []
            for cell in adj:
                if cell not in visited:
                    cost.append(self.heuristics(self.maze[cell[0]][cell[1]], end))
                    coords.append(cell)
            if len(cost) > 0:
                p_route.append(self.path[-1])
            elif len(p_route) > 0:
                coord = p_route.pop()
                popped = self.path.pop()
                while coord != popped:
                    popped = self.path.pop()
                self.path.append(coord)
                adj = self.get_adjacent_cells(coord[0], coord[1])
                cost = []
                coords = []
                for cell in adj:
                    if cell not in visited:
                        cost.append(self.heuristics(self.maze[cell[0]][cell[1]], end))
                        coords.append(cell)
            if len(cost) > 0:
                self.path.append(coords[cost.index(min(cost))])
            visited.append(self.path[-1])
            w += 1
            if visited[-1] == (self.size - 1, self.size - 1):
                found = True
                self.visited = visited
                break

    def heuristics(self, cell1, cell2):
        # Returns the manhattan distance between the two cells
        return abs(cell1.x - cell2.x) + abs(cell1.y - cell2.y)

    def get_adjacent_cells(self, x, y):
        adj_cells = []
        walls = self.maze[x][y].get_walls()
        if walls[0] == False:
            adj_cells.append((x - 1, y))
        if walls[1] == False:
            adj_cells.append((x + 1, y))
        if walls[2] == False:
            adj_cells.append((x, y - 1))
        if walls[3] == False:
            adj_cells.append((x, y + 1))
        return adj_cells

    def get_path(self):
        return self.path

    def get_visited(self):
        return self.visited


if __name__ == '__main__':
    # initialize pygame
    pygame.init()
    clock = pygame.time.Clock()
    window = pygame.display.set_mode((width, height))
    window.fill(black)
    pygame.display.set_caption("Test 1")

    # Calls the Main function
    main()

    # Mainloop
    while True:
        # Sets frame rate to 60 FPS
        clock.tick(FPS)
        for event in pygame.event.get():
            if event.type == QUIT:
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    sys.exit()

        # Updates the display
        pygame.display.update()

# main()

