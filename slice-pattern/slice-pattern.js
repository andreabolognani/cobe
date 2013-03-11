/* slice-pattern.js - Draw a colorful pattern on a canvas
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

function debug(message) {

	$("#debug").text(message);
}

function debugAppend(message) {

	$("#debug").text($("#debug").text() + " " + message);
}

function inRange(v, min, max) {

	while (v < min) {
		v += Math.abs(max - min + 1);
	}

	while (v > max) {
		v -= Math.abs(max - min + 1);
	}

	return v;
}

function normalizeAngle(angle) {

	return inRange(angle, 0, 359);
}

function toRadians(degrees) {

	return degrees / 180 * Math.PI;
}

function distance(a, b) {

	return Math.sqrt(Math.pow((b.x - a.x), 2) + Math.pow((b.y - a.y), 2));
}

function maxDistance(points) {

	var max;
	var len;

	max = 0;
	len = points.length;

	for (var i = 1; i < len; ++i) {
		max = Math.max(distance(points[0], points[i]), max);
	}

	return max;
}

function drawBeachBall(args) {

	var canvas = args.canvas;
	var colors = args.colors;
	var slices = args.slices;
	var origin = args.origin;
	var radius = args.radius;
	var angleOffset = args.angleOffset;
	var start;
	var end;
	var i;

	slice = 360 / slices;

	start = normalizeAngle(angleOffset);
	end = normalizeAngle(start + slice);

	for (i = 0; i < slices; i++) {

		var color = colors[inRange(i, 0, 1)];

		canvas.drawArc({
			strokeStyle: color,
			fillStyle: color,
			closed: true,
			x: origin.x,
			y: origin.y,
			radius: radius,
			start: start,
			end: end
		});

		var x2 = origin.x + radius * Math.sin(toRadians(start));
		var y2 = origin.y - radius * Math.cos(toRadians(start));
		var x3 = origin.x + radius * Math.sin(toRadians(end));
		var y3 = origin.y - radius * Math.cos(toRadians(end));

		canvas.drawLine({
			fillStyle: color,
			closed: true,
			x1: origin.x,
			y1: origin.y,
			x2: x2,
			y2: y2,
			x3: x3,
			y3: y3
		});

		start = normalizeAngle(start + slice);
		end = normalizeAngle(end + slice);
	}
}

function drawFrame(args) {

	var slices = args.slices;
	var origin = args.origin;
	var radiusOffset = args.radiusOffset;
	var radiusIncrement = args.radiusIncrement;
	var angleOffset = args.angleOffset;

	var radius;
	var angleIncrement;

	angleIncrement = 360 / slices;

	// Calculate the maximum radius needed to completely fill the canvas
	radius = maxDistance([
		origin,
		{ x: 0, y: 0 },
		{ x: args.canvas.width(), y: 0 },
		{ x: args.canvas.width(), y: args.canvas.height() },
		{ x: 0, y: args.canvas.height() }
	]);
	radius = (Math.floor(radius / radiusIncrement) + 1);
	radius = radius * radiusIncrement + radiusOffset;

	while (radius > 0) {

		drawBeachBall({
			canvas: args.canvas,
			colors: args.colors,
			slices: slices,
			origin: origin,
			radius: radius,
			angleOffset: angleOffset,
		});

		// Decrease the radius to draw a smaller circle, increase
		// the angle so that colors get automatically swapped
		radius -= radiusIncrement;
		angleOffset += angleIncrement;
	}
}

function main() {

	// Configurable values
	const COLOR_LIGHT = "#F22477";
	const COLOR_DARK = "#003040";
	const SLICES = 40;
	const RADIUS_INCREMENT = 70;

	var canvas;
	var colors;
	var origin;
	var radiusOffset;

	if (SLICES % 2 != 0) {
		debug("The number of slices must be even!");
		return;
	}

	canvas = $("canvas");
	colors = [
		COLOR_LIGHT,
		COLOR_DARK
	];

	// Draw the first frame starting from the center of the canvas
	origin = {
		x : Math.floor(canvas.width() / 2),
		y : Math.floor(canvas.height() / 2)
	};

	angleOffset = 0;
	radiusOffset = 0;

	// React to clicks by moving the origin
	canvas.click(function(e) {

		origin = {
			x: e.pageX - e.target.offsetLeft,
			y: e.pageY - e.target.offsetTop
		};

		drawFrame({
			canvas: canvas,
			colors: colors,
			slices: SLICES,
			origin: origin,
			angleOffset: angleOffset,
			radiusOffset: radiusOffset,
			radiusIncrement: RADIUS_INCREMENT
		});
	});

	// Animate by continuously changing angle and radius
	window.setInterval(function() {

		var radius = radiusOffset + 3;
		radiusOffset = inRange(radiusOffset + 3, 0, RADIUS_INCREMENT - 1);
		angleOffset = inRange(angleOffset + 1, 0, 359);

		// Switch colors
		if (radiusOffset != radius) {
			var tmp = colors[0];
			colors[0] = colors[1];
			colors[1] = tmp;
		}

		drawFrame({
			canvas: canvas,
			colors: colors,
			slices: SLICES,
			origin: origin,
			angleOffset: angleOffset,
			radiusOffset: radiusOffset,
			radiusIncrement: RADIUS_INCREMENT
		});
	}, 100);
}

$(document).ready(main);
