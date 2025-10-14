import random
import copy

SIMULATION_COUNT = 5  # 1手につきn回ランダムシミュレーションする
MAX_PLAYOUT_DEPTH = 5  # 1手につき最大n手だけランダムで進める

def choose_move(player, board, players, player_index):
    best_move = None
    best_score = -float('inf')

    # まず置けるブロックを「大きい順」にソート
    sorted_blocks = sorted(
    player.usable_blocks,
    key=lambda b: -block_size(b[0])  # b[0]がshape_index
    )


    for move in sorted_blocks:
        shape_index, direction_index, positions = move
        if not positions:
            continue
        x, y = random.choice(positions)  # 置く位置はランダムでOK

        total_score = 0
        for _ in range(SIMULATION_COUNT):
            sim_board = copy.deepcopy(board)
            sim_players = copy.deepcopy(players)

            if not try_place(sim_board, sim_players[player_index],
                             shape_index, direction_index, x, y):
                continue

            score = random_playout(sim_board, sim_players, player_index, player_index)
            total_score += score

        avg_score = total_score / SIMULATION_COUNT
        if avg_score > best_score:
            best_score = avg_score
            best_move = move

    return best_move

def random_playout(board, players, current_player_index, target_player_index):
    depth = 0
    while depth < MAX_PLAYOUT_DEPTH:
        current_player = players[current_player_index]

        # 置ける手を全部集める
        moves = []
        for shape_index, direction_index, positions in current_player.usable_blocks:
            for x, y in positions:
                if board.settable_check(current_player.color, shape_index, x, y):
                    moves.append((shape_index, direction_index, x, y))

        if not moves:
            # パス
            current_player_index = (current_player_index + 1) % len(players)
            depth += 1
            continue

        # ランダムに1手選んで置く
        shape_index, direction_index, x, y = random.choice(moves)
        try_place(board, current_player, shape_index, direction_index, x, y)

        current_player_index = (current_player_index + 1) % len(players)
        depth += 1

    # 最後の盤面で target_player 視点のスコア差を返す
    return players[target_player_index].score - players[1 - target_player_index].score

def try_place(board, player, shape_index, direction_index, x, y, show=False):
    import Block
    block = Block.Block(shape_index, direction_index, need_to_show=show)
    if not board.settable_check(player.color, block.selected['shape'], x, y):
        return False
    board.change_status(player.color, block.selected['shape'], block.selected['influence'], x, y)
    return True

def is_game_over(board, players):
    for player in players:
        for _, _, positions in player.usable_blocks:
            if positions:
                return False
    return True

def block_size(shape_index):
    import Block
    block = Block.Block(shape_index, 0, need_to_show=False)  # 必須引数を追加
    # shape の中で1が置けるマスの数を数える
    return (block.selected['shape'] != 0).sum()

