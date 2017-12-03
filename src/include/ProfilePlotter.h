#ifndef CLASS_NCINE_PROFILEPLOTTER
#define CLASS_NCINE_PROFILEPLOTTER

#include "Rect.h"
#include "Color.h"
#include "nctl/Array.h"
#include "nctl/StaticArray.h"
#include "DrawableNode.h"
#include "PlottingVariable.h"

namespace ncine {

/// A class that plots a graphic representation of a time/value function
class ProfilePlotter : public DrawableNode
{
  public:
	ProfilePlotter(SceneNode *parent, Rectf rect);
	~ProfilePlotter() override;

	/// Adds a value to the specified variable
	bool addValue(unsigned int varIndex, float value);
	/// Returns the variable with the specified index
	PlottingVariable &variable(unsigned int index);
	/// Sets the backgound color for the graphs
	void setBackgroundColor(Color backgroundColor) { backgroundColor_ = backgroundColor; }

	/// Returns the reference value line color
	inline const Color &refValueColor() const { return refValueColor_; }
	/// Returns the state of the reference value drawing flag
	inline bool shouldPlotRefValue() const { return shouldPlotRefValue_; }
	/// Returns the reference value
	inline float refValue() const { return refValue_; }

	/// Sets the reference value line color
	inline void setRefValueColor(Color refValueColor) { refValueColor_ = refValueColor; }
	/// Sets the reference value drawing flag state
	inline void setPlotRefValue(bool enabled) { shouldPlotRefValue_ = enabled; }
	/// Sets the reference value
	inline void setRefValue(float value) { refValue_ = value; }
	/// Returns the reference value normalized and clamped between `min` and `max`
	float normBetweenRefValue(float min, float max) const;

	inline virtual void drawRefValue(RenderQueue &renderQueue)
	{
		UpdateRefValueRenderCommand();
		renderQueue.addCommand(&refValueCmd_);
	}

	/// Adds a new variable to the plotter
	virtual unsigned int addVariable(unsigned int numValues, float rejectDelay) = 0;

  protected:
	/// Background color
	Color backgroundColor_;
	/// The vertices for the background
	nctl::StaticArray<float, 8> backgroundVertices_; // Quad with a triangle strip
	/// The array of variables
	nctl::Array<PlottingVariable *> variables_;

	/// Reference value drawing flag
	bool shouldPlotRefValue_;
	/// Reference value line color
	Color refValueColor_;
	/// The vertices for the reference value line
	nctl::StaticArray<float, 4> refValueVertices_;
	/// The reference value
	float refValue_;
	/// The command used to render the reference value
	RenderCommand refValueCmd_;

	/// Fills the background buffer with vertices
	void setBackgroundVertices();
	void updateRenderCommand() override;
	/// Updates the reference value rendering command
	void UpdateRefValueRenderCommand();

  private:
	/// Deleted copy constructor
	ProfilePlotter(const ProfilePlotter &) = delete;
	/// Deleted assignment operator
	ProfilePlotter &operator=(const ProfilePlotter &) = delete;
};

}

#endif
