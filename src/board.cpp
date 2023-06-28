#include "board.h"

#include <algorithm>
#include <vector>
#include "pieces.h"

namespace bblocks::board
{
    namespace
    {
        constexpr uint32_t  board_line_width{ 6 }; // Width of each of the two lines that delimit the board
        constexpr uint32_t  block_size{ 16 }; // Width and Height of each block of a piece
        constexpr uint32_t  board_width{ 10 }; // Board width in blocks
        constexpr uint32_t  board_height{ 20 }; // Board height in blocks
        constexpr uint32_t  min_vertical_margin{ 20 }; // Minimum vertical margin for the board limit
        constexpr uint32_t  min_horizontal_margin{ 20 }; // Minimum horizontal margin for the board limit
        constexpr uint32_t  piece_blocks{ 5 }; // Number of horizontal and vertical blocks of a matrix piece

    	std::vector<uint32_t>	lines_to_delete_;
        bool					lines_for_deletion_{ false };
        uint32_t				board_[board_width][board_height]{};
        uint32_t				screen_height_{ 0 };
        uint32_t				screen_width_{ 0 };
        uint32_t				board_position_{ 0 };
        uint32_t				lines_deleted_{ 0 };

        void
    	delete_line(const uint32_t y, float dt)
        {
            // Moves all the upper lines one row down
            for (uint32_t j{ y }; j > 0; --j)
                for (uint32_t i{ 0 }; i < board_width; ++i)
                    board_[i][j] = board_[i][j - 1];

            ++lines_deleted_;
        }
    } // anonymous

    void
	init(const uint32_t width, const uint32_t height)
    {
        screen_width_ = width;
        screen_height_ = height;
        board_position_ = (screen_width_ / 2);
		lines_to_delete_.clear();
		lines_for_deletion_ = false;
		lines_deleted_ = 0;

        for (int i{ 0 }; i < static_cast<int>(board_width); ++i)
            for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
                board_[i][j] = 0;
    }

	uint32_t
	get_x_pos_in_pixels(const uint32_t pos)
	{
        return ((board_position_ - (block_size * (board_width / 2))) + (pos * block_size));
	}

    uint32_t
	get_y_pos_in_pixels(const uint32_t pos)
    {
        return ((screen_height_ - (block_size * board_height)) + (pos * block_size));
    }

    uint32_t
	get_board_position()
    {
        return board_position_;
    }

	bool
	is_free_block(const uint32_t x, const uint32_t y)
    {
        return board_[x][y] == 0;
    }

	uint32_t
	get_piece_type(const uint32_t x, const uint32_t y)
    {
        return board_[x][y] - 1;
    }
    
    bool
	is_possible_movement(const int32_t x, const int32_t y, const uint32_t piece, const uint32_t rotation)
    {
	    // Checks collision with pieces already stored in the board or the board limits
	    // This is just to check the 5×5 blocks of a piece with the appropriate area in the board
	    for (int32_t i1{ x }, i2{ 0 }; i1 < x + static_cast<int32_t>(piece_blocks); ++i1, ++i2)
		    for (int32_t j1{ y }, j2{ 0 }; j1 < y + static_cast<int32_t>(piece_blocks); ++j1, ++j2)
		    {
			    // Check if the piece is outside the limits of the board
			    if (i1 < 0 || i1 > static_cast<int32_t>(board_width) - 1 || j1 > static_cast<int32_t>(board_height) - 1)
				    if (piece::get_block_type(piece, rotation, j2, i2) != 0) return false;

			    // Check if the piece have collided with a block already stored in the map
			    if (j1 >= 0)
				    if ((piece::get_block_type(piece, rotation, j2, i2) != 0) && (!is_free_block(i1, j1)))
					    return false;
		    }

	    return true;
    }

    void
    store_piece(const int32_t x, const int32_t y, const uint32_t piece, const uint32_t rotation)
    {
	    // Store each block of the piece into the board
	    for (int32_t i1{ x }, i2{ 0 }; i1 < x + static_cast<int32_t>(piece_blocks); ++i1, ++i2)
		    for (int32_t j1{ y }, j2{ 0 }; j1 < y + static_cast<int32_t>(piece_blocks); ++j1, ++j2)
			    // Store only the blocks of the piece that are not holes
			    if (piece::get_block_type(piece, rotation, j2, i2) != 0)
				    board_[i1][j1] = piece + 1;
    }

    void
    delete_possible_lines(const float dt)
    {
	    static float delay{ 0.f };

	    for (int j{ 0 }; j < static_cast<int>(board_height); ++j)
	    {
		    int i{ 0 };
		    while (i < static_cast<int>(board_width))
		    {
			    if (board_[i][j] <= 0) break;
			    ++i;
		    }

		    if (i == board_width)
			    if (lines_to_delete_.empty() ||
				    std::find(lines_to_delete_.begin(), lines_to_delete_.end(), j) == lines_to_delete_.end())
			    {
				    lines_to_delete_.emplace_back(j);
				    lines_for_deletion_ = true;
			    }
	    }

	    if (lines_for_deletion_ && delay < .25f)
	    {
		    delay += dt;
		    return;
	    }

	    if (lines_for_deletion_)
	    {
		    for (uint32_t i{ 0 }; i < lines_to_delete_.size(); ++i)
			    delete_line(lines_to_delete_[i], dt);

		    lines_to_delete_.clear();
		    lines_for_deletion_ = false;
		    delay = 0.f;
	    }
    }
    
    bool
	is_game_over()
    {
        //If the first line has blocks, then, game over
        for (uint32_t i{ 0 }; i < board_width; ++i)
            if (board_[i][0] > 0) return true;

        return false;
    }

	uint32_t
	lines_deleted()
    {
        return lines_deleted_;
    }

	uint32_t
	get_board_line_width()
	{
        return board_line_width;
	}

	uint32_t
	get_block_size()
	{
		return block_size;
	}

	uint32_t
	get_board_width()
	{
		return board_width;
	}

	uint32_t
	get_board_height()
	{
		return board_height;
	}

	uint32_t
	get_min_vertical_margin()
	{
		return min_vertical_margin;
	}

	uint32_t
	get_min_horizontal_margin()
	{
		return min_horizontal_margin;
	}

	uint32_t
	get_piece_blocks()
	{
		return piece_blocks;
	}
}
