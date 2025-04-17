#include "Enemy.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

Enemy::Enemy(const sf::Vector2f& position)
    : Entity(position, ENEMY_SPEED, ENEMY_HEALTH)
{
    // Setup enemy shape
    mShape.setRadius(ENEMY_RADIUS);
    mShape.setFillColor(sf::Color::Red);
    mShape.setOrigin(ENEMY_RADIUS, ENEMY_RADIUS);
    mShape.setPosition(mPosition);
    
    // Setup health bar
    mHealthBarBackground.setSize(sf::Vector2f(60.f, 10.f));
    mHealthBarBackground.setFillColor(sf::Color(100, 100, 100));
    
    mHealthBar.setSize(sf::Vector2f(60.f, 10.f));
    mHealthBar.setFillColor(sf::Color::Red);
}

void Enemy::update(sf::Time deltaTime)
{
    // Update path to target at regular intervals
    if (mPathUpdateClock.getElapsedTime().asSeconds() >= PATH_UPDATE_TIME)
    {
        mPath = findPath(mPosition, mTargetPosition);
        mPathUpdateClock.restart();
    }
    
    // Move along path
    if (!mPath.empty())
    {
        sf::Vector2f nextPoint = mPath[0];
        sf::Vector2f direction = nextPoint - mPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance < 5.f) // If we're close to the point, go to the next one
        {
            mPath.erase(mPath.begin());
        }
        else
        {
            // Normalize the direction
            direction.x /= distance;
            direction.y /= distance;
            
            // Apply movement
            mVelocity = direction;
        }
    }
    else
    {
        // If no path, move directly towards target
        sf::Vector2f direction = mTargetPosition - mPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0)
        {
            direction.x /= distance;
            direction.y /= distance;
            mVelocity = direction;
        }
        else
        {
            mVelocity = sf::Vector2f(0.f, 0.f);
        }
    }
    
    // Apply velocity
    mPosition += mVelocity * mSpeed * deltaTime.asSeconds();
    mShape.setPosition(mPosition);
    
    // Update health bar position
    mHealthBarBackground.setPosition(mPosition.x - 30.f, mPosition.y - ENEMY_RADIUS - 20.f);
    mHealthBar.setPosition(mPosition.x - 30.f, mPosition.y - ENEMY_RADIUS - 20.f);
    mHealthBar.setSize(sf::Vector2f(60.f * static_cast<float>(mHealth) / ENEMY_HEALTH, 10.f));
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(mShape);
    renderHealthBar(window);
}

void Enemy::setTargetPosition(const sf::Vector2f& target)
{
    mTargetPosition = target;
}

void Enemy::renderHealthBar(sf::RenderWindow& window)
{
    window.draw(mHealthBarBackground);
    window.draw(mHealthBar);
}

std::vector<sf::Vector2f> Enemy::findPath(const sf::Vector2f& start, const sf::Vector2f& goal)
{
    // Convert world positions to grid
    sf::Vector2i startNode = worldToGrid(start);
    sf::Vector2i goalNode = worldToGrid(goal);
    
    // A* algorithm
    std::vector<sf::Vector2f> path;
    
    // If start and goal are the same, return empty path
    if (startNode == goalNode)
    {
        return path;
    }
    
    // Priority queue for open nodes
    struct NodeCost {
        sf::Vector2i pos;
        float fScore;
        
        bool operator<(const NodeCost& other) const {
            return fScore > other.fScore; // Min-heap
        }
    };
    
    std::priority_queue<NodeCost> openSet;
    openSet.push({startNode, heuristic(startNode, goalNode)});
    
    // Maps to track node costs and paths
    std::unordered_map<sf::Vector2i, sf::Vector2i, Vec2Hash> cameFrom;
    std::unordered_map<sf::Vector2i, float, Vec2Hash> gScore;
    
    // Initialize costs
    gScore[startNode] = 0.f;
    
    while (!openSet.empty())
    {
        sf::Vector2i current = openSet.top().pos;
        openSet.pop();
        
        // If we found the goal
        if (current == goalNode)
        {
            // Reconstruct path
            std::vector<sf::Vector2i> gridPath;
            while (current != startNode)
            {
                gridPath.push_back(current);
                current = cameFrom[current];
            }
            
            // Convert grid path to world coordinates and reverse
            std::reverse(gridPath.begin(), gridPath.end());
            for (auto& node : gridPath)
            {
                path.push_back(gridToWorld(node));
            }
            
            return path;
        }
        
        // Check neighbors
        for (auto& neighbor : getNeighbors(current))
        {
            float tentativeG = gScore[current] + 1.f; // Assuming uniform cost
            
            if (!gScore.count(neighbor) || tentativeG < gScore[neighbor])
            {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeG;
                float fScore = tentativeG + heuristic(neighbor, goalNode);
                openSet.push({neighbor, fScore});
            }
        }
    }
    
    // No path found
    return path;
}

std::vector<sf::Vector2i> Enemy::getNeighbors(const sf::Vector2i& current)
{
    // Define neighbor offsets (4-way movement)
    static const std::vector<sf::Vector2i> offsets = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };
    
    std::vector<sf::Vector2i> neighbors;
    neighbors.reserve(offsets.size());
    
    for (const auto& offset : offsets)
    {
        sf::Vector2i neighbor = current + offset;
        
        // Add valid neighbors (for now, just assume all cells are traversable)
        // In a real game, you would check for obstacles here
        neighbors.push_back(neighbor);
    }
    
    return neighbors;
}

float Enemy::heuristic(const sf::Vector2i& a, const sf::Vector2i& b)
{
    // Manhattan distance
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

sf::Vector2i Enemy::worldToGrid(const sf::Vector2f& worldPos)
{
    return sf::Vector2i(
        static_cast<int>(worldPos.x / GRID_CELL_SIZE),
        static_cast<int>(worldPos.y / GRID_CELL_SIZE)
    );
}

sf::Vector2f Enemy::gridToWorld(const sf::Vector2i& gridPos)
{
    return sf::Vector2f(
        (gridPos.x + 0.5f) * GRID_CELL_SIZE,
        (gridPos.y + 0.5f) * GRID_CELL_SIZE
    );
} 