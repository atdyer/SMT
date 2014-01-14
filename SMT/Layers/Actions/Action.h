#ifndef ACTION_H
#define ACTION_H

/**
 * @brief The Action class is a virtual class that allows us to implement
 * undo and redo stacks in the SelectionLayer.
 *
 * The Action class is a virtual class that allows us to implement undo and
 * redo stacks in the SelectionLayer. A subclass of the Action class must
 * override the RedoAction and UndoAction functions. The RedoAction function
 * is called when the user clicks the redo button and the UndoAction function
 * is called when the user clicks the undo button.
 *
 */
class Action
{
	public:
		Action();

		virtual void	RedoAction() = 0;
		virtual void	UndoAction() = 0;
};

#endif // ACTION_H
