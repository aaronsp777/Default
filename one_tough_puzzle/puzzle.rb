#!/usr/bin/ruby

class Side
  attr_accessor :suit, :outtie

  def initialize(outtie, suit)
    @outtie = outtie
    @suit = suit
  end

  def is_compatible(other)
    return suit == other.suit &&
      outtie != other.outtie
  end

  def to_s
    if outtie
      return suit.upcase
    else
      return suit.downcase
    end
  end
end

class Piece
  attr_accessor :rotation, :sides
  def initialize(top, right, bottom, left)
    @rotation = 0
    @sides = []
    sides << Side.new(true, top)
    sides << Side.new(false, right)
    sides << Side.new(false, bottom)
    sides << Side.new(true, left)
  end

  def top
    return sides[rotation & 3]
  end

  def right
    return sides[(rotation + 1) & 3]
  end

  def bottom
    return sides[(rotation + 2) & 3]
  end

  def left
    return sides[(rotation + 3) & 3]
  end

  def to_s
    sprintf "[%s%s%s%s]", top, right, bottom, left
  end
end

class Board
  attr_accessor :grid

  def initialize(placements, rotations, pieces)
    @grid = []
    left = pieces.dup
    until left.empty?
      size = left.length
      pick = placements % size
      placements = placements / size
      grid << left.delete_at(pick)
      grid.last.rotation = rotations & 3
      rotations = rotations / 4
    end
  end

  def solved?
    return true &&
      grid[0].right.is_compatible(grid[1].left) &&
      grid[1].right.is_compatible(grid[2].left) &&

      grid[3].right.is_compatible(grid[4].left) &&
      grid[4].right.is_compatible(grid[5].left) &&

      grid[6].right.is_compatible(grid[7].left) &&
      grid[7].right.is_compatible(grid[8].left) &&

      grid[0].bottom.is_compatible(grid[3].top) &&
      grid[1].bottom.is_compatible(grid[4].top) &&
      grid[2].bottom.is_compatible(grid[5].top) &&

      grid[3].bottom.is_compatible(grid[6].top) &&
      grid[4].bottom.is_compatible(grid[7].top) &&
      grid[5].bottom.is_compatible(grid[8].top)
  end

  def to_s
    return sprintf "%s%s%s %s%s%s %s%s%s",
      grid[0], grid[1], grid[2],
      grid[3], grid[4], grid[5],
      grid[6], grid[7], grid[8]
  end
end

# main

pieces = []
pieces << Piece.new("S","C","H","S")
pieces << Piece.new("D","D","C","C")
pieces << Piece.new("H","C","C","D")

pieces << Piece.new("H","H","D","D")
pieces << Piece.new("C","H","S","H")
pieces << Piece.new("C","C","D","H")

pieces << Piece.new("S","H","S","D")
pieces << Piece.new("H","C","S","S")
pieces << Piece.new("S","D","H","D")

0.upto(1*2*3*4*5*6*7*8*9-1) do |placements|
  0.upto(4**9-1) do |rotations|
    b = Board.new(placements, rotations, pieces)

    if b.solved?
      puts "WIN: " + b.to_s
    end
  end
end


# Only one answer: (4 rotations)
# WIN: [SchS][DdcC][HccD] [HhdD][ChsH][CcdH] [DShs][SHcs][DSdh]
# WIN: [ccDH][cdHC][SdhD] [dcCD][hsHC][HcsS] [chSS][hdDH][ShsD]
# WIN: [sDSh][DHhd][SSch] [sSHc][HChs][CDdc] [hDSd][HCcd][DHcc]
# WIN: [dhDS][csSH][hsDS] [dHCc][sHCh][dDHh] [cDHc][cCDd][hSSc]
