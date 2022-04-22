#pragma once

#include "QtWidgets/QCheckBox"
#include "qpainter.h"
#include "QtCore"

class ToggleButton: public QCheckBox
{
	Q_PROPERTY(QColor bodyColor READ getBodyColor WRITE setBodyColor)
	Q_PROPERTY(QColor bodyHoverColor READ getBodyHoverColor WRITE setBodyHoverColor)
	Q_PROPERTY(QColor iconColor READ getIconColor WRITE setIconColor)

public:
	// constructor
	ToggleButton(QWidget *parent = Q_NULLPTR);

	// setters
	void setBodyColor(QColor color) { bodyColor = color; currentBodyColor = color; }
	void setBodyHoverColor(QColor color) { bodyHoverColor = color; }
	void setIconColor(QColor color) { iconColor = color; currentIconColor = color; }

	// getters
	QColor getBodyColor() const { return bodyColor; }
	QColor getBodyHoverColor() const { return bodyHoverColor; }
	QColor getIconColor() const { return iconColor; }
	QRect getBodyRect() const;

	virtual void paintEvent(QPaintEvent *event) override;
	bool hitButton(const QPoint& pos) const { return contentsRect().contains(pos); }

protected:
	void resizeEvent(QResizeEvent* event);

private:
	QColor bodyColor = QColor("#dddddd");
	QColor bodyHoverColor = QColor("#2c2c2c");
	QColor indicatorColor = QColor("#ffffff");
	QColor iconColor = QColor("#e8e8e8");
	QColor currentBodyColor = bodyColor;
	QColor currentIconColor = iconColor;
	QEasingCurve easingCurve = QEasingCurve::InOutSine;
	int bodyXPos = 0;
	int iconWidth = 0;
	int animationDuration = 500;

	QVariantAnimation* getCurrentBodyColorAnimation();
	QVariantAnimation* getBodyXPosAnimation();
	QVariantAnimation* getCurrentIconColorAnimation();
	QVariantAnimation* getIconWidthAnimation();

private slots:
	void startAnimation();
};
