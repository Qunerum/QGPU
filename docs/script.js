let docData = null, currentLang = localStorage.getItem('selectedLang') || 'en';
async function loadDocumentation() {
	try {
		const response = await fetch('data.json');
		if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
		docData = await response.json();
		updateFunctionsFromJSON();
		if (!localStorage.getItem('selectedLang')) currentLang = docData.defaultLanguage || 'en';
		renderDocs();
	} catch (error) { console.error("JSON Error:", error); }
}
const syntaxRules = {
	'code-comment': ['//.*'],
	'code-string': ['".*?"', '\'.*?\''],
	'code-number': ['\\b\\d+(?:\\.\\d+)?f?\\b'],
	'code-define': [
		'MAX_VERTICES','MAX_LIGHTS','MAX_UI_VERTICES','VSYNC','SHADOW_MAP_SIZE',
		'QGPU_STYLE_REGULAR','QGPU_STYLE_BOLD','QGPU_STYLE_ITALIC','QGPU_STYLE_UNDERLINE','QGPU_STYLE_STRIKETHROUGH',
		'QGPU_SHOW_BANNER','QGPU_SHOW_MADE_WITH_QGPU','QGPU_SHOW_INFO','QGPU_SHOW_COLORS','QGPU_SHOW_LOGS',
		'QGPU_SETTINGS_AMBIENT_OCCLUSION','QGPU_SETTINGS_MSAA_LEVEL','QGPU_SETTINGS_SHADOWS',
		'QGPU_FONT_STYLE_REGULAR','QGPU_FONT_STYLE_BOLD','QGPU_FONT_STYLE_ITALIC','QGPU_FONT_STYLE_BOLD_ITALIC',
		'LMB','RMB','QKEY_A','QKEY_B','QKEY_C','QKEY_D','QKEY_E','QKEY_F','QKEY_G','QKEY_H','QKEY_I','QKEY_J',
		'QKEY_K','QKEY_L','QKEY_M','QKEY_N','QKEY_O','QKEY_P','QKEY_Q','QKEY_R','QKEY_S','QKEY_T','QKEY_U',
		'QKEY_V','QKEY_W','QKEY_X','QKEY_Y','QKEY_Z','QKEY_SPACE','QKEY_ESCAPE','QKEY_ENTER','QKEY_BACKSPACE',
		'QKEY_LSHIFT','QKEY_LCTRL','QKEY_RIGHT','QKEY_LEFT','QKEY_DOWN','QKEY_UP',
		'RST','REGULAR','BOLD','BLACK','WHITE','LIGHT_GRAY','LIGHT_RED','LIGHT_GREEN','LIGHT_YELLOW',
		'LIGHT_ORANGE','LIGHT_BLUE','LIGHT_MAGENTA','LIGHT_CYAN','GRAY','RED','GREEN','YELLOW','ORANGE',
		'BLUE','MAGENTA','CYAN','DARK_GRAY','DARK_RED','DARK_GREEN','DARK_YELLOW','DARK_ORANGE',
		'DARK_BLUE','DARK_MAGENTA','DARK_CYAN',
		'ITALIC','UNDERLINE','STRIKETHROUGH','BOLD_ITALIC',
		'MADE_WITH_QGPU','INFO','COLORS','LOGS','MSAA_LEVEL','SHADOWS'
	],
	'code-keyword': [
		'void','if','else','return','for','while',
		'const','int','float','uint','uint8_t','Vector2','Vector3'
	],
	'code-function': []
};
function updateFunctionsFromJSON() {
	if (!docData || !docData.functions) return;
	const extractedNames = [];
	Object.values(docData.functions).forEach(category => { if (Array.isArray(category)) category.forEach(fn => { if (fn.name) extractedNames.push(fn.name); }); });
	syntaxRules['code-function'] = extractedNames;
}
function highlightCode(codeText) {
	if (!codeText) return '';
	let escaped = codeText
		.replace(/&/g, '&amp;')
		.replace(/</g, '&lt;')
		.replace(/>/g, '&gt;');
	const regexParts = Object.keys(syntaxRules).map(className => {
		const patterns = syntaxRules[className];
		if (!patterns || patterns.length === 0) return null;
		const groupName = className.replace(/-/g, '_'),
		formattedPatterns = patterns.map(pattern => {
			const isRegex = /[\\[\\^$.|?*+(){}]/.test(pattern);
			return isRegex ? pattern : `\\b${pattern}\\b`;
		});
		return `(?<${groupName}>${formattedPatterns.join('|')})`;
	}).filter(Boolean);
	if (regexParts.length === 0) return escaped;
	const masterRegex = new RegExp(regexParts.join('|'), 'g');
	return escaped.replace(masterRegex, (match, ...args) => {
		const groups = args[args.length - 1];
		if (groups.code_string) {
			const formattedString = match.replace(/%[a-zA-Z]/g, '<span class="code-format-specifier">$&</span>');
			return `<span class="code-string">${formattedString}</span>`;
		}
		for (const [groupName, value] of Object.entries(groups)) {
			if (value !== undefined) {
				const className = groupName.replace(/_/g, '-');
				return `<span class="${className}">${match}</span>`;
			}
		}
		return match;
	});
}
function renderDocs() {
	if (!docData) return;
	const titleText = getTranslation(docData.title);
	document.title = titleText;
	const titleElement = document.getElementById('doc-title');
	if (titleElement) titleElement.textContent = titleText;
	const categoriesText = document.getElementById('categories'),
	sidebarList = document.getElementById('category-list'),
	mainContainer = document.getElementById('docs-container');
	if (categoriesText) categoriesText.innerHTML = getTranslation(docData.categories);
	if (sidebarList) sidebarList.innerHTML = '';
	if (mainContainer) mainContainer.innerHTML = '';
	if (docData.functions) {
		const categories = Object.keys(docData.functions);
		categories.forEach(categoryName => {
			const funcList = docData.functions[categoryName];
			if (sidebarList) {
				const navItem = document.createElement('a');
				navItem.href = '#';
				navItem.textContent = categoryName;
				navItem.addEventListener('click', (e) => {
					e.preventDefault();
					const targetSection = document.getElementById(`category-${categoryName}`);
					if (targetSection) targetSection.scrollIntoView({ behavior: 'smooth', block: 'start' });
				});
				sidebarList.appendChild(navItem);
			}
			if (mainContainer) {
				const section = document.createElement('section');
				section.className = 'category-section';
				section.id = `category-${categoryName}`;
				const catHeader = document.createElement('h2');
				catHeader.className = 'category-title';
				catHeader.textContent = categoryName;
				section.appendChild(catHeader);
				if (Array.isArray(funcList)) {
					funcList.forEach(fn => {
						let formattedArgs = '';
						if (fn.args) {
							formattedArgs = Object.keys(fn.args).map(argKey => {
								const argDescObj = fn.args[argKey],
									tooltipText = getTranslation(argDescObj),
									parts = argKey.trim().split(/\s+/);
								let argHTML = '';
								if (parts.length > 1) {
									const type = parts.slice(0, -1).join(' '), name = parts[parts.length - 1];
									argHTML = `<span class="syntax-arg-type">${type}</span> <span class="syntax-arg-name">${name}</span>`;
								} else argHTML = `<span class="syntax-arg">${argKey}</span>`;
								const highlightedTooltip = highlightCode(tooltipText);
								return `<span class="arg-wrapper">${argHTML}<span class="tooltip-box">${highlightedTooltip}</span></span>`;
							}).join('<span class="syntax-punct">, </span>');
						}
						const funcContainer = document.createElement('div');
						funcContainer.className = 'func-container';
						const hasExample = Array.isArray(fn.example) && fn.example.length > 0,
							description = getTranslation(fn.description),
							funcRow = document.createElement('div');
						funcRow.className = 'func-row';
						let arrowHTML = hasExample ? `<button class="func-toggle-btn" title="Pokaż przykład"><span class="arrow">▼</span></button>` : '';
						funcRow.innerHTML = `
						<div class="func-main-info">
						<code>
						<span class="syntax-return">${fn.returnType}</span>
						<span class="syntax-name">${fn.name}</span><span class="syntax-punct">(</span>${formattedArgs}<span class="syntax-punct">);</span>
						</code>
						<span class="func-separator"></span>
						<span class="func-desc">${description}</span>
						</div>
						${arrowHTML}
						`;
						funcContainer.appendChild(funcRow);
						if (hasExample) {
							const exampleBox = document.createElement('div');
								exampleBox.className = 'func-example-box';
								const rawCode = Array.isArray(fn.example) ? fn.example.join('\n') : fn.example,
							highlightedCode = highlightCode(rawCode);
							exampleBox.innerHTML = `<pre><code>${highlightedCode}</code></pre>`;
							funcContainer.appendChild(exampleBox);
							const toggleBtn = funcRow.querySelector('.func-toggle-btn');
							toggleBtn.addEventListener('click', () => {
								const isOpen = funcContainer.classList.toggle('is-open');
								toggleBtn.classList.toggle('active', isOpen);
							});
						}
						section.appendChild(funcContainer);
					});
				}
				mainContainer.appendChild(section);
			}
		});
	}
}
function getTranslation(translatableObject) {
	if (!translatableObject) return '';
	return translatableObject[currentLang]
		|| translatableObject[docData.defaultLanguage]
		|| Object.values(translatableObject)[0]
		|| 'NULL';
}
function changeLanguage(lang) {
	currentLang = lang;
	localStorage.setItem('selectedLang', lang);
	renderDocs();
}
document.addEventListener('DOMContentLoaded', loadDocumentation);
