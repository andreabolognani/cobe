/* psycho-circles.js - Draw a colorful animated pattern on a canvas
 * Copyright (C) 2013  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */


// -----------------
//  Utility methods
// -----------------

var Util;

Util = {}

// Convert degrees to radians

Util.toRadians = function(degrees) {

	return degrees / 180 * Math.PI;
}

// Force a value within a well-defined minimum and maximum

Util.inRange = function(v, min, max) {

	while (v < min) {
		v += Math.abs(max - min + 1);
	}

	while (v > max) {
		v -= Math.abs(max - min + 1);
	}

	return v;
}

// Normalize an angle (expressed in degrees)

Util.normalizeAngle = function(degrees) {

	return Util.inRange(degrees, 0, 359);
}

// Calculate the distance between two points

Util.distance = function(a, b) {

	return Math.sqrt(Math.pow((b.x - a.x), 2) + Math.pow((b.y - a.y), 2));
}

// Calculate the distance between a reference point and the
// farthest in a list of other points 

Util.maxDistance = function(origin, points) {

	var max;
	var len;

	max = 0;
	len = points.length;

	for (var i = 0; i < len; ++i) {
		max = Math.max(Util.distance(origin, points[i]), max);
	}

	return max;
}


// -----------------
//  Animation class
// -----------------

var Animation;

Animation = function(canvas) {

	// Settings

	this.colors = [
		"#223366",
		"#3377bb",
		"#77aabb",
		"#ccdddd",
		"#99cccc"
	];
	this.slices = 30;
	this.partRadius = 60;
	this.partAngle = 360 / this.slices;
	this.frameRadiusIncrement = 2;
	this.frameAngleIncrement = 2;
	this.frameInterval = 20;

	// State

	this.canvas = canvas;
	this.canvasWidth = canvas.width();
	this.canvasHeight = canvas.height();
	this.origin = {
		x: Math.floor(this.canvasWidth / 2),
		y: Math.floor(this.canvasHeight / 2)
	};
	this.running = false;
	this.angle = 0;
	this.radius = 0;
}

Animation.prototype.update = function() {

	// Increase radius and angle by the configured amount

	this.radius = Util.inRange(this.radius + this.frameRadiusIncrement, 0, this.partRadius - 1);
	this.angle = Util.normalizeAngle(this.angle + this.frameAngleIncrement);

	// Adjust angle when the numer of parts changes so that the
	// transition between frames is smooth

	if (this.radius <= 0) {
		this.angle = Util.normalizeAngle(this.angle + this.partAngle);
	}
}

Animation.prototype.paint = function() {
	new Frame(this).paint();
}


// -------------
//  Frame class
// -------------

var Frame;

Frame = function(animation) {

	this.animation = animation;
}

Frame.prototype.paintPart = function(radius, angle) {

	var start;
	var end;

	start = Util.normalizeAngle(angle);
	end = Util.normalizeAngle(start + this.animation.partAngle);

	for (var i = 0; i < this.animation.slices; i++) {

		var color;
		var x2;
		var y2;
		var x3;
		var y3;

		// Cycle through all the available colors

		color = this.animation.colors[Util.inRange(i, 0, this.animation.colors.length - 1)];

		this.animation.canvas.drawArc({
			strokeStyle: color,
			fillStyle: color,
			closed: true,
			x: this.animation.origin.x,
			y: this.animation.origin.y,
			radius: radius,
			start: start,
			end: end
		});

		x2 = this.animation.origin.x + radius * Math.sin(Util.toRadians(start));
		y2 = this.animation.origin.y - radius * Math.cos(Util.toRadians(start));
		x3 = this.animation.origin.x + radius * Math.sin(Util.toRadians(end));
		y3 = this.animation.origin.y - radius * Math.cos(Util.toRadians(end));

		this.animation.canvas.drawLine({
			fillStyle: color,
			closed: true,
			x1: this.animation.origin.x,
			y1: this.animation.origin.y,
			x2: x2,
			y2: y2,
			x3: x3,
			y3: y3
		});

		start = Util.normalizeAngle(start + this.animation.partAngle);
		end = Util.normalizeAngle(end + this.animation.partAngle);
	}
}

Frame.prototype.paint = function() {

	var angle;
	var radius;

	angle = this.animation.angle;

	// Calculate the minimum radius needed to completely fill the canvas

	radius = Util.maxDistance(
		this.animation.origin,
		[
			{ x: 0, y: 0 },
			{ x: this.animation.canvasWidth, y: 0 },
			{ x: this.animation.canvasWidth, y: this.animation.canvasHeight },
			{ x: 0, y: this.animation.canvasHeight }
		]
	);
	radius = Math.floor(radius / this.animation.partRadius) + 1;
	radius *= this.animation.partRadius;
	radius += this.animation.radius;

	while (radius > 0) {

		this.paintPart(radius, angle);

		// Decrease the radius to draw a smaller pattern, increase
		// the angle to shift the colors

		angle = Util.normalizeAngle(angle + this.animation.partAngle);
		radius -= this.animation.partRadius;
	}
}


// -------------
//  Entry point
// -------------

function main() {

	var animation;
	var running;

	animation = new Animation($("#canvas"));
	running = $("#running").button();

	window.setInterval(function() {

		// Only update when the animation is running

		if (animation.running) {
			animation.update();
			animation.paint();
		}
	}, animation.frameInterval);

	animation.canvas.click(function(e) {

		// Move the origin to the click location

		animation.origin = {
			x: e.pageX - e.target.offsetLeft,
			y: e.pageY - e.target.offsetTop
		};

		animation.paint();
	});

	running.click(function() {
		animation.running = !animation.running;
	});

	// Draw the first frame

	animation.paint();
}

$(document).ready(main);
