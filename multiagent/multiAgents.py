# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for 
# educational purposes provided that (1) you do not distribute or publish 
# solutions, (2) you retain this notice, and (3) you provide clear 
# attribution to UC Berkeley, including a link to 
# http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero 
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and 
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        foodPosition = newFood.asList()
        costList=[]
        for element in foodPosition:
          costList.append(util.manhattanDistance(element,newPos))
        if len(costList)==0:
          disToFood=0
        else:
          disToFood=1.0/(min(costList))

        dis=util.manhattanDistance(ghostState.getPosition(),newPos)
        power=successorGameState.getCapsules()
        powerDis=[]
        for element in power:
          powerDis.append(util.manhattanDistance(element,newPos))
        
        totalTime=0
        for element in newScaredTimes:
          totalTime+=element
        if len(powerDis)==0:
          powerFactor=0
        else:
          powerFactor=1.0/min(powerDis)



        '''print successorGameState.getScore(),newPos,newFood,newGhostStates,newScaredTimes'''
        
        return 1000*successorGameState.getScore()+disToFood+dis/50 + powerFactor+ totalTime 

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        depth = 0
        actionDict = {}
        agent = 0
        def pacmanValue(gameState, depth, agent=0):
            agent = 0
            numGhosts = gameState.getNumAgents() - 1
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)
            v = -float("inf")
            pacmanActions = gameState.getLegalActions(0)
            for action in pacmanActions:
                successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent)
                v = max(v, successor)
            return v

        def ghostValue(gameState, depth, agent):
            numGhosts = gameState.getNumAgents() - 1
            agent = agent + 1
            if agent == numGhosts:
                depth = depth + 1
            v = float("inf")
            ghostActions = gameState.getLegalActions(agent)
            if not ghostActions:
                return self.evaluationFunction(gameState)
            if agent < numGhosts:
                for action in ghostActions:
                    successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent)
                    v = min(v, successor)
            else:
                for action in ghostActions:
                    successor = pacmanValue(gameState.generateSuccessor(agent, action), depth, agent)
                    v = min(v, successor)
            return v
        
        for action in gameState.getLegalActions(0):
            successor = gameState.generateSuccessor(0, action)
            value = ghostValue(successor, depth, 0)
            actionDict[action] = value
        maxValue = pacmanValue(gameState, depth)
        for action in gameState.getLegalActions(0):
            if actionDict[action] == maxValue:
                return action






                      






        



        

        
class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        depth = 0
        actionDict = {}
        agent = 0
        alpha = -float("inf")
        beta = float("inf")
        def pacmanValue(gameState, depth, agent, alpha, beta):
            agent = 0
            numGhosts = gameState.getNumAgents() - 1
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)
            v = -float("inf")
            pacmanActions = gameState.getLegalActions(0)
            for action in pacmanActions:
                successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent, alpha, beta)
                v = max(v, successor)
                if v > beta:
                    return v
                alpha = max(alpha, v)
            return v

        def ghostValue(gameState, depth, agent, alpha, beta):
            numGhosts = gameState.getNumAgents() - 1
            agent = agent + 1
            if agent == numGhosts:
                depth = depth + 1
            v = float("inf")
            ghostActions = gameState.getLegalActions(agent)
            if not ghostActions:
                return self.evaluationFunction(gameState)
            if agent < numGhosts:
                for action in ghostActions:
                    successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent, alpha, beta)
                    v = min(v, successor)
                    if v < alpha:
                        return v
                    beta = min(beta, v)
            else:
                for action in ghostActions:
                    successor = pacmanValue(gameState.generateSuccessor(agent, action), depth, agent, alpha, beta)
                    v = min(v, successor)
                    if v < alpha:
                        return v
                    beta = min(beta, v)
            return v
        for action in gameState.getLegalActions(0):
            successor = gameState.generateSuccessor(0, action)
            value = ghostValue(successor, depth, 0, alpha, beta)
            alpha = max(alpha, value)
            actionDict[action] = value
            
        maxValue = pacmanValue(gameState, depth, agent, alpha, beta)
        
        for action in gameState.getLegalActions(0):
            if actionDict[action] == maxValue:
                return action
        


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        depth = 0
        actionDict = {}
        agent = 0
        def pacmanValue(gameState, depth, agent=0):
            agent = 0
            numGhosts = gameState.getNumAgents() - 1
            if depth == self.depth or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)
            v = -float("inf")
            pacmanActions = gameState.getLegalActions(0)
            for action in pacmanActions:
                successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent)
                v = max(v, successor)
            return v

        def ghostValue(gameState, depth, agent):
            numGhosts = gameState.getNumAgents() - 1
            agent = agent + 1
            if agent == numGhosts:
                depth = depth + 1
            v = 0
            ghostActions = gameState.getLegalActions(agent)
            if not ghostActions:
                return self.evaluationFunction(gameState)
            if agent < numGhosts:
                for action in ghostActions:
                    successor = ghostValue(gameState.generateSuccessor(agent, action), depth, agent)
                    p = 1.0/len(ghostActions)
                    v += p*successor
            else:
                for action in ghostActions:
                    successor = pacmanValue(gameState.generateSuccessor(agent, action), depth, agent)
                    p = 1.0/len(ghostActions)
                    v +=  p * successor
            return v
        for action in gameState.getLegalActions(0):
            successor = gameState.generateSuccessor(0, action)
            value = ghostValue(successor, depth, 0)
            actionDict[action] = value
        maxValue = pacmanValue(gameState, depth)
        for action in gameState.getLegalActions(0):
            if actionDict[action] == maxValue:
                return action


def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>

      I find the manhattanDistance from the pacman to the ghosts and the distance to the closest food and the distance 
      to the closest powerpellet and the scared time. Take the reciprocal of the the distance to the closest food and 
      the reciprocal of the distance to the closest powerpellet. give current game score a factor of 1000 and give
      the distance to food a factor of 1/200. Adding all these 5 features together is my evaluation 
      function.  
    """
    "*** YOUR CODE HERE ***"
    newPos=currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()
    newGhostStates = currentGameState.getGhostStates()
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

    foodPosition = newFood.asList()
    costList=[]
    for element in foodPosition:
        costList.append(util.manhattanDistance(element,newPos))
    if len(costList)==0:
        disToFood=0
    else:
        disToFood=1.0/(min(costList))

    dis=util.manhattanDistance(ghostState.getPosition(),newPos)
    power=currentGameState.getCapsules()
    powerDis=[]
    for element in power:
        powerDis.append(util.manhattanDistance(element,newPos))
        
    totalTime=0
    for element in newScaredTimes:
        totalTime+=element
    if len(powerDis)==0:
        powerFactor=0
    else:
        powerFactor=1.0/min(powerDis)



        
        
    return 1000*currentGameState.getScore()+disToFood+dis/200 + powerFactor+ totalTime 



# Abbreviation
better = betterEvaluationFunction

